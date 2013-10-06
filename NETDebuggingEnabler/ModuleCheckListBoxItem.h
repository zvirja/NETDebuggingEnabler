#pragma once
#include "baseset.h"

class ModuleCheckListBoxItem:public wxClientData
{
public:
	ModuleCheckListBoxItem(const wxString& fullModuleName) { this->FullModulePath = FullModulePath; }
	
	wxString& GetFullModulePath(){ return FullModulePath; }
private:
	wxString FullModulePath;
};

