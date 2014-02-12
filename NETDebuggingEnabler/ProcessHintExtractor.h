#pragma once;
#include "ProcessInfo.h"

namespace winternlAPI
{
	#include <winternl.h>
}


namespace ProcessHintExtractorStuff
{
	typedef 	NTSTATUS
		(NTAPI *PNtQueryInformationProcess)
		(
		IN HANDLE ProcessHandle,
		IN winternlAPI::PROCESSINFOCLASS ProcessInformationClass,
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
		ProcessHintExtractorStuff::PNtQueryInformationProcess NtQueryInformationProcessMethod;
		wxString ExtractHintFromParams(const wxString& rawCommandLine);
	};
}