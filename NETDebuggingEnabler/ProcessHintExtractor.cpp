#include "baseset.h"
#include "ProcessHintExtractor.h"
#include <memory>
#include <wx\cmdline.h>

using namespace std;
using namespace ProcessHintExtractorStuff;

namespace Managers
{

	ProcessHintExtractor::ProcessHintExtractor()
	{
		hNtDll = LoadLibrary(L"ntdll.dll");
		if (hNtDll == nullptr)
			return;
		NtQueryInformationProcess = (PNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");
		if (NtQueryInformationProcess == nullptr) {
			FreeLibrary(hNtDll);
			hNtDll = nullptr;
			return;
		}
	}

	ProcessHintExtractor::~ProcessHintExtractor()
	{
		if (hNtDll != nullptr)
			FreeLibrary(hNtDll);
	}


	void ProcessHintExtractor::FillProcessInfoWithHint(DWORD processID, Frames::ProcessInfo& processInfo)
	{

		//Check if import was successful
		if (NtQueryInformationProcess == nullptr)
			return;

		//Apply this filter only to w3wp
		/*if (!processInfo.GetName().Upper().Contains("W3WP"))
			return;*/

		//Open process
		unique_handle process(OpenProcess(PROCESS_ALL_ACCESS, false, processID));
		if (process.get() == INVALID_HANDLE_VALUE || process.get() == NULL)
			return;

		//Get basic info
		PROCESS_BASIC_INFORMATION procBasicInfo;
		ULONG resLen;
		if (NtQueryInformationProcess(process.get(), ProcessBasicInformation, &procBasicInfo, sizeof(procBasicInfo), &resLen) != ERROR_SUCCESS)
			return;

		//Extract PEB from foreign memory
		PEB peb;
		auto ss = sizeof(peb);
		size_t pebResLen;
		if (!ReadProcessMemory(process.get(), procBasicInfo.PebBaseAddress, &peb, sizeof(PEB), &pebResLen))
			return;

		//Extract process parameters from foreign memory
		RTL_USER_PROCESS_PARAMETERS procParams;
		if (!ReadProcessMemory(process.get(), peb.ProcessParameters, &procParams, sizeof(RTL_USER_PROCESS_PARAMETERS), &pebResLen))
			return;

		//Extract command line parameters
		UNICODE_STRING& commandLine = procParams.CommandLine;
		size_t buffSize = commandLine.Length + 2;
		unique_ptr<wchar_t[]> commandLineBuffer = unique_ptr<wchar_t[]>(new wchar_t[buffSize]);
		if (!ReadProcessMemory(process.get(), commandLine.Buffer, commandLineBuffer.get(), buffSize, &pebResLen))
			return;


		//Stuff to clear later
		wxString sttrr(commandLineBuffer.get());


 		int a = 10;
	}



}