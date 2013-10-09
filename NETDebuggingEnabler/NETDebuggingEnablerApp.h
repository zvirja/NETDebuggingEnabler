#pragma once
#include <wx/wx.h>
#include "Configuration.h"

class NETDebuggingEnablerApp :public wxApp
{
public:
	static Managers::Configuration *AppConfig;
	NETDebuggingEnablerApp();
	~NETDebuggingEnablerApp();
	virtual bool OnInit();
};

