#pragma once
#include <wtypes.h>

namespace Managers
{
	class PrivilegiesAdjuster
	{
	public:
		static bool SetDebugPrevilege();
	private:
		static bool SetPrivilege(
			HANDLE hToken,          // access token handle
			LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
			bool bEnablePrivilege   // to enable or disable privilege
			);
	};
}
