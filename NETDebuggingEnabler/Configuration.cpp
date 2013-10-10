#include "baseset.h"
#include "Configuration.h"

using namespace std;

namespace Managers
{

	wxString Configuration::KeyOnlyNet = L"DisplayOnlyNET";
	wxString Configuration::KeyEnableProcessNameFilter = L"UseProcessNameFilter";
	wxString Configuration::KeyProcessNameFilter = L"ProcessNameFilter";
	wxString Configuration::KeyDisplayFullModulePaths = L"DisplayFullModulePaths";
	wxString Configuration::KeyEnableModulePathFilter = L"UseModulePathFilter";
	wxString Configuration::KeyModulePathFilter = L"ModulePathFilter";

	Configuration::Configuration() :actualConfig(new wxRegConfig(L"NETDebuggingEnabler", L"Zvirja", 
		wxEmptyString, wxEmptyString, wxCONFIG_USE_LOCAL_FILE))
	{		 
	}


	Configuration::~Configuration()
	{
	}

	bool Configuration::GetBoolValue(const wxString& key, bool fallbackValue)
	{
		return actualConfig->ReadBool(key, fallbackValue);
	}

	void Configuration::SetBoolValue(const wxString& key, bool value)
	{
		actualConfig->Write(key, value);
	}

	wxString Configuration::GetStringValue(const wxString& key, const wxString& fallbackValue)
	{
		return actualConfig->Read(key, fallbackValue);
	}

	void Configuration::SetStringValue(const wxString& key, const wxString& value)
	{
		actualConfig->Write(key, value);
	}

}