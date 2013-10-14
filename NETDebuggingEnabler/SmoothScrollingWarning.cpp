#include "baseset.h"
#include "SmoothScrollingWarning.h"
#include <windows.h>
#include "NETDebuggingEnablerApp.h"

namespace Managers
{
	void SmoothScrollingWarning::WarnIfEnabledOnce()
	{
		BOOL actualValue;
		if (SystemParametersInfo(SPI_GETLISTBOXSMOOTHSCROLLING, 0, &actualValue, 0))
		{
			if (actualValue != FALSE && NETDebuggingEnablerApp::AppConfig->RequestForSmoothScrollingWarning())
			{
				if (wxMessageBox(L"PLEASE READ THIS TO THE END and sorry for this warning!\r\n\r\nWe have detected that \"Smooth-scroll list boxes\" setting is enabled on your PC.\r\nThis may significantly slow the speed of controls scrolling.\r\nIt's recommended to test scrolling with enabled/disabled setting value and choose the best option.\r\n\r\nDo you want to get details about this setting and how to disable it?",
					L"NETDebuggingEnabler -- Performance related warning", wxYES_NO | wxYES_DEFAULT | wxICON_EXCLAMATION) == wxYES)
				{
					wxLaunchDefaultBrowser(L"http://alexpovar.blogspot.com/2013/10/what-is-windows-smooth-scroll-list.html");
				}
			}
		}
	}

}