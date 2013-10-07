#pragma once
#include "baseset.h"

namespace Managers
{
	class OptimizationController
	{
	public:
		OptimizationController(const wxString& assemblyPath);
		static bool IsOptimizationDisabledForAssembly(const wxString& assemblyPath);
		bool DisableOptimization();
		bool EnableOptimization();
		bool IsOptimizationDisabled() { return isDisabled; }
	private:
		wxString iniPath;
		bool isDisabled;
		static wxString GetIniPathFromAssemblyPath(const wxString& assemblyPath);
	};
}