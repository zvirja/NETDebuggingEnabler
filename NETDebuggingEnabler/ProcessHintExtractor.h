#pragma once;
#include "ProcessInfo.h"

namespace ProcessHintExtractorStuff
{
	enum PROCESSINFOCLASS {
		ProcessBasicInformation = 0,
		ProcessDebugPort = 7,
		ProcessWow64Information = 26,
		ProcessImageFileName = 27,
		ProcessBreakOnTermination = 29
	};

	struct PEB_LDR_DATA {
		BYTE Reserved1[8];
		PVOID Reserved2[3];
		LIST_ENTRY InMemoryOrderModuleList;
	};

	struct RTL_USER_PROCESS_PARAMETERS {
		BYTE Reserved1[16];
		PVOID Reserved2[10];
		UNICODE_STRING ImagePathName;
		UNICODE_STRING CommandLine;
	};

	typedef
		VOID
		(NTAPI *PPS_POST_PROCESS_INIT_ROUTINE) (
		VOID
		);

	struct PEB {
		BYTE Reserved1[2];
		BYTE BeingDebugged;
		BYTE Reserved2[1];
		PVOID Reserved3[2];
		PEB_LDR_DATA* Ldr;
		RTL_USER_PROCESS_PARAMETERS* ProcessParameters;
		PVOID Reserved4[3];
		PVOID AtlThunkSListPtr;
		PVOID Reserved5;
		ULONG Reserved6;
		PVOID Reserved7;
		ULONG Reserved8;
		ULONG AtlThunkSListPtr32;
		PVOID Reserved9[45];
		BYTE Reserved10[96];
		PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
		BYTE Reserved11[128];
		PVOID Reserved12[1];
		ULONG SessionId;
	};

	struct PROCESS_BASIC_INFORMATION {
		PVOID Reserved1;
		PEB* PebBaseAddress;
		PVOID Reserved2[2];
		ULONG_PTR UniqueProcessId;
		PVOID Reserved3;
	};

	typedef 	NTSTATUS
		(NTAPI *PNtQueryInformationProcess)
		(
		IN HANDLE ProcessHandle,
		IN PROCESSINFOCLASS ProcessInformationClass,
		OUT PVOID ProcessInformation,
		IN ULONG ProcessInformationLength,
		OUT PULONG ReturnLength OPTIONAL
		);

}

namespace Managers
{
	class ProcessHintExtractor
	{
	public:
		ProcessHintExtractor();
		~ProcessHintExtractor();
		void FillProcessInfoWithHint(DWORD processID, ProcessInfo& processInfo);
	private:
		HMODULE  hNtDll;
		ProcessHintExtractorStuff::PNtQueryInformationProcess NtQueryInformationProcess;
		wxString ExtractHintFromParams(const wxString& rawCommandLine);
	};
}