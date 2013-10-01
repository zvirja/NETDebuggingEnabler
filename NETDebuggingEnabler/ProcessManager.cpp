#pragma comment(lib, "psapi.lib")
#include "ProcessManager.h"
#include <TlHelp32.h>
#include <Psapi.h>
#include <sstream>

using namespace std;
using namespace Frames;

namespace Managers
{
	ProcessManager::ProcessManager()
	{
	}

	ProcessManager::~ProcessManager()
	{
	}

	vector<ProcessInfo> ProcessManager::GetProcessesList()
	{
		HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
			return vector<ProcessInfo>();
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (!Process32First(hProcessSnap, &pe32))
		{
			CloseHandle(hProcessSnap);
			return vector<ProcessInfo>();
		}

		vector<ProcessInfo> resultList;
		do
		{
			if (pe32.th32ProcessID != 0)
				resultList.push_back(ProcessInfo(wxString(pe32.szExeFile), pe32.th32ProcessID));
		} while (Process32Next(hProcessSnap, &pe32));

		CloseHandle(hProcessSnap);
		return resultList;
	}

	std::vector<wxString> ProcessManager::GetModulesForProcessId(int processID, DWORD& resultCode)
	{
		HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
		resultCode = ERROR_SUCCESS;
		if (process == NULL)
		{
			resultCode = GetLastError();
			return vector<wxString>();
		}
		
		HMODULE modules[10];
		DWORD needed;
		auto size = sizeof(modules);
		if (!EnumProcessModulesEx(process, modules, sizeof(modules), &needed, LIST_MODULES_ALL))
		{
			CloseHandle(process);
			resultCode = GetLastError();
			return vector<wxString>();
		}
		vector<wxString> test;

		HMODULE* modulesSource;
		unique_ptr<HMODULE> dynamicSource;
		if (needed > sizeof(modules))
		{
			dynamicSource = unique_ptr<HMODULE>( new HMODULE[needed / sizeof(HMODULE)]);
			if (!EnumProcessModulesEx(process, dynamicSource.get(), needed, &needed, LIST_MODULES_ALL))
			{
				CloseHandle(process);
				resultCode = GetLastError();
				return vector<wxString>();
			}
			modulesSource = dynamicSource.get();
		}
		else
		{
			modulesSource = modules;
		}
		int arrayLength = sizeof(modules) / sizeof(HMODULE);
		int arrivedLength = needed / sizeof(HMODULE);
		
		wchar_t nameBuffer[1000];

		for (int i = 0; i < arrivedLength; i++)
		{
			if (GetModuleFileNameEx(process, modulesSource[i], nameBuffer, sizeof(nameBuffer)))
			{
				test.push_back(nameBuffer);
			}
		}
		return test;
	}
}