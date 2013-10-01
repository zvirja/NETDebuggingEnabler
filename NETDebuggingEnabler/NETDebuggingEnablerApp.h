#pragma once
#include <wx/wx.h>

class NETDebuggingEnablerApp:public wxApp
{
public:
	NETDebuggingEnablerApp();
	~NETDebuggingEnablerApp();
	virtual bool OnInit();
};

