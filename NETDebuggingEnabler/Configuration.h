#pragma once
#include "baseset.h"
#include <wx/confbase.h>
#include <wx/msw/regconf.h>
namespace Managers
{
	class Configuration
	{
	public:
		Configuration();
		~Configuration();
		bool GetOnlyNetProcesses() { return GetBoolValue(KeyOnlyNet, false); }
		void SetOnlyNetProcesses(bool value) { SetBoolValue(KeyOnlyNet, value); }
		bool GetEnableProcessNameFilter() { return GetBoolValue(KeyEnableProcessNameFilter, false); }
		void SetEnableProcessNameFilter(bool value) { SetBoolValue(KeyEnableProcessNameFilter, value); }
		wxString GetProcessNameFilter() { return GetStringValue(KeyProcessNameFilter, L"w3wp"); }
		void SetProcessNameFilter(const wxString& value) { SetStringValue(KeyProcessNameFilter, value); }
		bool GetDisplayFullModulePaths() { return GetBoolValue(KeyDisplayFullModulePaths, false); }
		void SetDisplayFullModulePaths(bool value) { SetBoolValue(KeyDisplayFullModulePaths, value); }
		bool GetEnableModulePathFilter() { return GetBoolValue(KeyEnableModulePathFilter, false); }
		void SetEnableModulePathFilter(bool value) { SetBoolValue(KeyEnableModulePathFilter, value); }
		wxString GetModulePathFilter() { return GetStringValue(KeyModulePathFilter, L"*assembly\\dl3*"); }
		void SetModulePathFilter(const wxString& value) { SetStringValue(KeyModulePathFilter, value); }

		wxPoint GetLastPosition();
		void SetLastPosition(const wxPoint& pos);
		wxSize GetLastSize();
		void SaveLastSize(const wxSize& size);
		bool RequestForSmoothScrollingWarning();

	private:
		static wxString KeyOnlyNet;
		static wxString KeyEnableProcessNameFilter;
		static wxString KeyProcessNameFilter;
		static wxString KeyDisplayFullModulePaths;
		static wxString KeyEnableModulePathFilter;
		static wxString KeyModulePathFilter;
		static wxString KeyPosX;
		static wxString KeyPosY;
		static wxString KeySizeW;
		static wxString KeySizeH;
		static wxString KeySmoothScrollingWarning;



		std::unique_ptr<wxConfigBase> actualConfig;

		bool GetBoolValue(const wxString& key, bool fallbackValue);
		void SetBoolValue(const wxString& key, bool value);
		wxString GetStringValue(const wxString& key, const wxString& fallbackValue);
		void SetStringValue(const wxString& key, const wxString& value);
	};
}