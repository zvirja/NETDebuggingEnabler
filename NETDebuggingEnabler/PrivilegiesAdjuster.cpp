#include "baseset.h"
#include "PrivilegiesAdjuster.h"
#include <processthreadsapi.h>
namespace Managers
{

	bool PrivilegiesAdjuster::SetDebugPrevilege()
	{
		unique_handle hCurrentProcess(GetCurrentProcess()); // 
		HANDLE hToken;
		BOOL obtainedProcessToken = OpenProcessToken(hCurrentProcess.get(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
		unique_handle tokenHandle(hToken);
		if (obtainedProcessToken)
		{
			bool result = SetPrivilege(hToken, SE_DEBUG_NAME, TRUE);
			if (result)
				return true;
		}
		wxMessageBox(wxT("Unable to escalate privilegies. Process will be unable to fetch modules for system apps. Probably isn't run as administrator."), wxT(".NET Debugging enabler"));
		return false;
	}

	bool PrivilegiesAdjuster::SetPrivilege(
		HANDLE hToken, /* access token handle */ 
		LPCTSTR lpszPrivilege, /* name of privilege to enable/disable */ 
		bool bEnablePrivilege /* to enable or disable privilege */)
	{
		LUID luid;

		if (!LookupPrivilegeValue(
			NULL,            // lookup privilege on local system
			lpszPrivilege,   // privilege to lookup 
			&luid))        // receives LUID of privilege
		{
			return false;
		}

		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		if (bEnablePrivilege)
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		else
			tp.Privileges[0].Attributes = 0;

		// Enable the privilege or disable all privileges.

		if (!AdjustTokenPrivileges(
			hToken,
			false,
			&tp,
			sizeof(TOKEN_PRIVILEGES),
			(PTOKEN_PRIVILEGES)nullptr,
			(PDWORD)nullptr))
		{
			return false;
		}

		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

		{
			return false;
		}

		return true;
	}

}