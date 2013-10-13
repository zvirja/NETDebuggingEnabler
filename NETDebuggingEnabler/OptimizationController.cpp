#include "baseset.h"
#pragma comment(lib, "Shlwapi.lib")
#include "OptimizationController.h"
#include <wx/filename.h>
#include <wx/file.h>
#include <windows.h>
#include <shlwapi.h>

namespace Managers
{
	size_t OptimizationController::IniFileSize = 75;

	wxString OptimizationController::IniFileContent = L"[.NET Framework Debugging Control]\r\nGenerateTrackingInfo=1\r\nAllowOptimize=0";

	OptimizationController::OptimizationController(const wxString& assemblyPath)
	{
		this->iniPath = GetIniPathFromAssemblyPath(assemblyPath);
		this->isDisabled = IsOptimizationDisabledForAssembly(assemblyPath);
	}

	bool OptimizationController::DisableOptimization()
	{
		//wxFile iniFile(this->iniPath, wxFile::OpenMode::write);
		unique_handle fileHandle(CreateFile(iniPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0));
		if (fileHandle.get() == INVALID_HANDLE_VALUE)
			return false;
		DWORD written;
		auto anciiString = IniFileContent.ToAscii();
		if (WriteFile(fileHandle.get(), anciiString.data(), anciiString.length(), &written, nullptr))
		{
			return true;
		}
		return false;
	}

	bool OptimizationController::EnableOptimization()
	{
		if (DeleteFile(iniPath))
		{
			this->isDisabled = false;
			return true;
		}
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			this->isDisabled = false;
			return true;
		}
		if (!PathFileExists(iniPath))
		{
			this->isDisabled = false;
			return true;
		}
		return false;
	}

	bool OptimizationController::IsOptimizationDisabledForAssembly(const wxString& assemblyPath)
	{
		wxString iniFilePath = GetIniPathFromAssemblyPath(assemblyPath);
		unique_handle fileHandle(CreateFile(iniFilePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0));
		if (fileHandle.get() == INVALID_HANDLE_VALUE)
			return false;
		LARGE_INTEGER fileSize;
		if (GetFileSizeEx(fileHandle.get(), &fileSize))
		{
			return fileSize.QuadPart == IniFileSize;
		}
		return false;
	}

	wxString OptimizationController::GetIniPathFromAssemblyPath(const wxString& assemblyPath)
	{
		wxFileName assemblyName(assemblyPath);
		auto root = assemblyName.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
		return wxString::Format(L"%s%s.ini", root, assemblyName.GetName());
	}
}