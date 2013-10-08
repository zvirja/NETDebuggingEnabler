#include "baseset.h"
#include "OptimizationController.h"
#include <wx/filename.h>
#include <wx/file.h>

namespace Managers
{
	OptimizationController::OptimizationController(const wxString& assemblyPath)
	{
		this->iniPath = GetIniPathFromAssemblyPath(assemblyPath);
		this->isDisabled = IsOptimizationDisabledForAssembly(assemblyPath);
	}

	bool OptimizationController::DisableOptimization()
	{
		wxFile iniFile(this->iniPath, wxFile::OpenMode::write);
		iniFile.Write(L"[.NET Framework Debugging Control]\r\nGenerateTrackingInfo=1\r\nAllowOptimize=0");
		iniFile.Close();
		return true;
	}

	bool OptimizationController::EnableOptimization()
	{
		if (wxFile::Exists(iniPath))
			wxRemoveFile(iniPath);
		this->isDisabled = false;
		return true;
	}

	bool OptimizationController::IsOptimizationDisabledForAssembly(const wxString& assemblyPath)
	{
		wxString iniFilePath = GetIniPathFromAssemblyPath(assemblyPath);
		if (!wxFile::Exists(iniFilePath))
			return false;
		auto size = wxFileName::GetSize(iniFilePath);
		return size == 75;
	}

	wxString OptimizationController::GetIniPathFromAssemblyPath(const wxString& assemblyPath)
	{
		wxFileName assemblyName(assemblyPath);
		auto root = assemblyName.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
		return wxString::Format(L"%s%s.ini", root, assemblyName.GetName());
	}
}