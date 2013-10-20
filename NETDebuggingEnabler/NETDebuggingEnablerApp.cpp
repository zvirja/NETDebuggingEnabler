#include "baseset.h"
#include "NETDebuggingEnablerApp.h"
#include "MainFrame.h"
#include "PrivilegiesAdjuster.h"
#include "SmoothScrollingWarning.h"

using namespace UI;
using namespace Managers;

Managers::Configuration* NETDebuggingEnablerApp::AppConfig = new Managers::Configuration();

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
	SmoothScrollingWarning::WarnIfEnabledOnce();
	MainFrame* mainFrame = new MainFrame;
	mainFrame->Show(true);
	return true;


}
