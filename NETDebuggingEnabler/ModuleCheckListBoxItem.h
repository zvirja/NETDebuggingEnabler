#pragma once
#include "baseset.h"
#include "OptimizationController.h"

namespace UI
{
	class ModuleCheckListBoxItem :public wxClientData
	{
	public:
		ModuleCheckListBoxItem(const wxString& fullModuleName) :optimizationController(fullModuleName)
		{
			this->FullModulePath = FullModulePath;
		}

		wxString& GetFullModulePath(){ return FullModulePath; }
		bool IsOptimizationDisabled() { return optimizationController.IsOptimizationDisabled(); }
		bool OptimizationDisabled(bool disableOptimization)
		{
			if (disableOptimization)
				return optimizationController.DisableOptimization();
			else
				return optimizationController.EnableOptimization();
		}
	private:
		wxString FullModulePath;
		Managers::OptimizationController optimizationController;
	};
}