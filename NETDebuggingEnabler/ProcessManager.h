#pragma once
#include <memory>
#include <vector>
#include <wx\wx.h>
#include "ProcessInfo.h"
#include <NTSecAPI.h>

namespace Managers
{
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
		std::vector<wxString> GetModulesForProcessId(int processID,DWORD& resultCode);
	private:
		HANDLE OpenProcessCustom(DWORD DesiredAccess, HANDLE dwProcessId);
	};
}
