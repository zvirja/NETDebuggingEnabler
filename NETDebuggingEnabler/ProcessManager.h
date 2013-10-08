#pragma once
#include <memory>
#include <vector>
#include <wx\wx.h>
#include "ProcessInfo.h"
#include <NTSecAPI.h>

namespace Managers
{

	struct wxStringOrdinalComparator {
		bool operator() (const wxString& lhs, const wxString& rhs) const{
			return lhs.CmpNoCase(rhs) < 0;
		}
	};

	BOOL CALLBACK EnumerateLoadedModulesProc64ProcManager(
		_In_ PCWSTR ModuleName,
		_In_ DWORD64 ModuleBase,
		_In_ ULONG ModuleSize,
		_In_opt_ PVOID UserContext
		);

	BOOL SetPrivilege(
		HANDLE hToken,          // access token handle
		LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
		BOOL bEnablePrivilege   // to enable or disable privilege
		);


	class ProcessManager
	{
	public:
		ProcessManager();
		~ProcessManager();
		std::vector<Frames::ProcessInfo> GetProcessesList();
		std::vector<wxString> GetModulesForProcessId(int processID, DWORD& resultCode, bool skipMappings = false);
	private:
		std::map<wxString, wxString> DriveMappings;
		std::set<wxString, wxStringOrdinalComparator> GetModulesForProcessIdInternalV1(int processID, DWORD& resultCode);
		std::set<wxString, wxStringOrdinalComparator> GetModulesForProcessIdInternalV2(int processID, DWORD& resultCode);
		std::set<wxString, wxStringOrdinalComparator> GetModulesForProcessIdInternalV3(int processID, DWORD& resultCode);
		void AppendMemoryMappedFiles(int processID, std::set<wxString, wxStringOrdinalComparator>& alreadyPresentModules);
		void InitDriveMappings();
		wxString FromDevicePathToRegular(const wxString& pathWithDevicePrefix);
	};
}