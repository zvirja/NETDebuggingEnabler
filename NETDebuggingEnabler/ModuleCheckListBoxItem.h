#pragma once
#include "baseset.h"
#include "OptimizationController.h"

class ModuleCheckListBoxItem:public wxClientData
{
public:
	ModuleCheckListBoxItem(const wxString& fullModuleName) :optimizationController(fullModuleName)
	{
		this->FullModulePath = FullModulePath; 
	}
	
	wxString& GetFullModulePath(){ return FullModulePath; }
	bool IsOptimizationDisabled() { return optimizationController.IsOptimizationDisabled(); }
	void OptimizationDisabled(bool disableOptimization) 
	{
		if (disableOptimization)
			optimizationController.DisableOptimization();
		else
			optimizationController.EnableOptimization();
	}
private:
	wxString FullModulePath;
	Managers::OptimizationController optimizationController;
};