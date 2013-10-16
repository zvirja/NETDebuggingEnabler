#include "baseset.h"
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "Dbghelp.lib")
#include "ProcessManager.h"
#include <TlHelp32.h>
#include <Psapi.h>
#include <sstream>
#include "unique_handle.h"
#include <array>
#include <DbgHelp.h>
#include <Windows.h>
#include <FileAPI.h>

using namespace std;
using namespace Frames;

namespace Managers
{
	ProcessManager::ProcessManager() :DriveMappings(), hintExtractor()
	{
		InitDriveMappings();
	}

	ProcessManager::~ProcessManager()
	{
	}

	void ProcessManager::InitDriveMappings()
	{
		wchar_t volumeName[100];
		wchar_t deviceName[150];
		wchar_t pathNames[100];

		HANDLE fileHandle = FindFirstVolumeW(volumeName, sizeof(volumeName));
		if (fileHandle == INVALID_HANDLE_VALUE)
			return;
		do
		{
			auto index = wcslen(volumeName) - 1;
			volumeName[index] = L'\0';
			auto charCount = QueryDosDeviceW(&volumeName[4], deviceName, sizeof(deviceName));
			volumeName[index] = L'\\';
			DWORD returned;
			if (GetVolumePathNamesForVolumeNameW(volumeName, pathNames, sizeof(pathNames), &returned))
			{
				DriveMappings[wxString(deviceName)] = wxString(pathNames);
			}


		} while (FindNextVolumeW(fileHandle, volumeName, sizeof(volumeName)));
		FindClose(fileHandle);
	}

	vector<ProcessInfo> ProcessManager::GetProcessesList()
	{
		unique_handle hProcessSnap(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
		if (hProcessSnap.get() == INVALID_HANDLE_VALUE)
			return vector<ProcessInfo>();
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (!Process32First(hProcessSnap.get(), &pe32))
		{
			return vector<ProcessInfo>();
		}

		vector<ProcessInfo> resultList;
		do
		{
			if (pe32.th32ProcessID != 0)
			{
				ProcessInfo pi(wxString(pe32.szExeFile), pe32.th32ProcessID);
				DWORD d;
				hintExtractor.FillProcessInfoWithHint(pe32.th32ProcessID, pi);
				resultList.push_back(move(pi));
			}
		} while (Process32Next(hProcessSnap.get(), &pe32));
		return move(resultList);
	}

	std::vector<wxString> ProcessManager::GetModulesForProcessId(int processID, DWORD& resultCode, bool skipMappings)
	{
		auto result = GetModulesForProcessIdInternalV1(processID, resultCode);
		if (!skipMappings)
			AppendMemoryMappedFiles(processID, result);
		vector<wxString> vectorResult;
		for (auto& modulePath : result)
			vectorResult.push_back(modulePath);
		return move(vectorResult);
	}

	std::set<wxString, wxStringOrdinalComparator> ProcessManager::GetModulesForProcessIdInternalV1(int processID, DWORD& resultCode)
	{
		unique_handle process(OpenProcess(PROCESS_ALL_ACCESS, false, processID));
		resultCode = ERROR_SUCCESS;
		if (process.get() == NULL)
		{
			resultCode = GetLastError();
			return set<wxString, wxStringOrdinalComparator>();
		}

		HMODULE modules[250];
		DWORD needed;
		auto size = sizeof(modules);
		if (!EnumProcessModulesEx(process.get(), modules, sizeof(modules), &needed, LIST_MODULES_ALL))
		{
			resultCode = GetLastError();
			return set<wxString, wxStringOrdinalComparator>();
		}
		set<wxString, wxStringOrdinalComparator> result;

		HMODULE* modulesSource;
		unique_ptr<HMODULE[]> dynamicSource;
		if (needed > sizeof(modules))
		{
			dynamicSource = unique_ptr<HMODULE[]>(new HMODULE[needed / sizeof(HMODULE)]);
			if (!EnumProcessModulesEx(process.get(), dynamicSource.get(), needed, &needed, LIST_MODULES_ALL))
			{
				resultCode = GetLastError();
				return set<wxString, wxStringOrdinalComparator>();
			}
			modulesSource = dynamicSource.get();
		}
		else
		{
			modulesSource = modules;
		}
		int arrayLength = sizeof(modules) / sizeof(HMODULE);
		int arrivedLength = needed / sizeof(HMODULE);

		wchar_t nameBuffer[250];

		for (int i = 0; i < arrivedLength; i++)
		{
			if (GetModuleFileNameEx(process.get(), modulesSource[i], nameBuffer, sizeof(nameBuffer)))
			{
				result.insert(nameBuffer);
			}
		}
		return move(result);
	}

	std::set<wxString, wxStringOrdinalComparator> ProcessManager::GetModulesForProcessIdInternalV2(int processID, DWORD& resultCode)
	{
		unique_handle process(OpenProcess(PROCESS_ALL_ACCESS, false, processID));
		resultCode = ERROR_SUCCESS;
		if (process.get() == NULL)
		{
			resultCode = GetLastError();
			return set<wxString, wxStringOrdinalComparator>();
		}
		unique_ptr<set<wxString, wxStringOrdinalComparator>> result = unique_ptr<set<wxString, wxStringOrdinalComparator>>(
			new set<wxString, wxStringOrdinalComparator>);
		//result->push_back(L"DUMMY");
		EnumerateLoadedModulesW64(process.get(), (PENUMLOADED_MODULES_CALLBACKW64)EnumerateLoadedModulesProc64ProcManager, result.get());
		return *result;
	}

	BOOL CALLBACK EnumerateLoadedModulesProc64ProcManager(_In_ PCWSTR ModuleName, _In_ DWORD64 ModuleBase, _In_ ULONG ModuleSize, _In_opt_ PVOID UserContext)
	{
		auto resultContainer = reinterpret_cast<set<wxString>*>(UserContext);
		//wxString vaaale(*ModuleName);
		resultContainer->insert(wxString(ModuleName));
		return true;
	}

	std::set<wxString, wxStringOrdinalComparator> ProcessManager::GetModulesForProcessIdInternalV3(int processID, DWORD& resultCode)
	{
		unique_handle hProcessSnap(CreateToolhelp32Snapshot(TH32CS_SNAPALL | TH32CS_SNAPMODULE32 | TH32CS_SNAPPROCESS, 0));
		if (hProcessSnap.get() == INVALID_HANDLE_VALUE)
			return set<wxString, wxStringOrdinalComparator>();
		MODULEENTRY32W me32;
		me32.dwSize = sizeof(MODULEENTRY32W);
		if (!Module32FirstW(hProcessSnap.get(), &me32))
		{
			return set<wxString, wxStringOrdinalComparator>();
		}

		set<wxString, wxStringOrdinalComparator> resultList;
		do
		{
			resultList.insert(me32.szExePath);
		} while (Module32NextW(hProcessSnap.get(), &me32));
		return move(resultList);
	}

	void ProcessManager::AppendMemoryMappedFiles(int processID, std::set<wxString, wxStringOrdinalComparator>& alreadyPresentModules)
	{
		unique_handle process(OpenProcess(PROCESS_ALL_ACCESS, false, processID));

		auto lastErrorCode = GetLastError();
		if ((lastErrorCode != ERROR_SUCCESS && lastErrorCode != ERROR_PARTIAL_COPY) || process.get() == NULL)
			return;
		MEMORY_BASIC_INFORMATION memInfo;
		size_t currentFrom = 0;
		wchar_t fileNameBuffer[500];
		while (VirtualQueryEx(process.get(), (void*)currentFrom, &memInfo, sizeof(memInfo)))
		{
			if (memInfo.Type == MEM_IMAGE)
			{
				if (GetMappedFileName(process.get(), memInfo.BaseAddress, fileNameBuffer, sizeof(fileNameBuffer)) > 0)
				{
					auto regularName = FromDevicePathToRegular(fileNameBuffer);
					alreadyPresentModules.insert(regularName);
				}
			}
			currentFrom += memInfo.RegionSize;
		}
	}

	wxString ProcessManager::FromDevicePathToRegular(const wxString& pathWithDevicePrefix)
	{
		for (auto entry : DriveMappings)
		{
			if (pathWithDevicePrefix.StartsWith(entry.first))
				return wxString::Format(L"%s%s", entry.second, pathWithDevicePrefix.Mid(entry.first.length() + 1));
		}
		return pathWithDevicePrefix;
	}
}