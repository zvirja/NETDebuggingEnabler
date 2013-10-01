#include "NETDebuggingEnablerApp.h"
#include "MainFrame.h"
#include "PrivilegiesAdjuster.h"
using namespace Frames;
using namespace Managers;

NETDebuggingEnablerApp::NETDebuggingEnablerApp()
{
	
}


NETDebuggingEnablerApp::~NETDebuggingEnablerApp()
{

}

bool NETDebuggingEnablerApp::OnInit()
{
	if (!PrivilegiesAdjuster::SetDebugPrevilege())
	{
		return false;
	}
	MainFrame* mainFrame = new MainFrame;
	mainFrame->Show(true);
	return true;
}
