#include "baseset.h"
#include "Configuration.h"
#include "MainFrame.h"

using namespace std;

namespace Managers
{

	wxString Configuration::KeyOnlyNet = L"DisplayOnlyNET";
	wxString Configuration::KeyEnableProcessNameFilter = L"UseProcessNameFilter";
	wxString Configuration::KeyProcessNameFilter = L"ProcessNameFilter";
	wxString Configuration::KeyDisplayFullModulePaths = L"DisplayFullModulePaths";
	wxString Configuration::KeyEnableModulePathFilter = L"UseModulePathFilter";
	wxString Configuration::KeyModulePathFilter = L"ModulePathFilter";
	wxString Configuration::KeyPosX = L"LastPositionX";
	wxString Configuration::KeyPosY = L"LastPositionY";
	wxString Configuration::KeySizeW = L"LastSizeW";
	wxString Configuration::KeySizeH = L"LastSizeH";
	wxString Configuration::KeySmoothScrollingWarning = L"SmoothScrollingWarningWasDisplayed";

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

	void Configuration::SetBoolValue(const wxString& key, bool value, bool defaultValue)
	{
		if(GetBoolValue(key,defaultValue) != value)
		{
			if(value == defaultValue)
				actualConfig->DeleteEntry(key);
			else
				actualConfig->Write(key, value);
		}
	}

	wxString Configuration::GetStringValue(const wxString& key, const wxString& fallbackValue)
	{
		return actualConfig->Read(key, fallbackValue);
	}

	void Configuration::SetStringValue(const wxString& key, const wxString& value, const wxString& defalutValue)
	{
		if(GetStringValue(key,defalutValue).CmpNoCase(value) != 0)
		{
			if(value.CmpNoCase(defalutValue) == 0)
				actualConfig->DeleteEntry(key);
			else
				actualConfig->Write(key, value);
		}
	}

	void Configuration::SetLastPosition(const wxPoint& pos)
	{
		if (pos.x > 0)
			actualConfig->Write(KeyPosX, pos.x);
		if (pos.y > 0)
			actualConfig->Write(KeyPosY, pos.y);
	}

	wxPoint Configuration::GetLastPosition()
	{

		int lastX, lastY;
		actualConfig->Read(KeyPosX, &lastX, 150);
		actualConfig->Read(KeyPosY, &lastY, 150);
		return wxPoint(lastX, lastY);
	}

	wxSize Configuration::GetLastSize()
	{
		int lastW, lastH;
		int defaultW = UI::MainFrame::FrameWidth;
		int defaultH = UI::MainFrame::FrameHeight;
		actualConfig->Read(KeySizeW, &lastW, defaultW);
		actualConfig->Read(KeySizeH, &lastH, defaultH);
		return wxSize(lastW < defaultW ? defaultW : lastW, lastH < defaultH ? defaultH : lastH);
	}

	void Configuration::SaveLastSize(const wxSize& size)
	{
		if (size.GetX() > 0)
			actualConfig->Write(KeySizeW, size.GetX());
		if (size.GetY() > 0)
			actualConfig->Write(KeySizeH, size.GetY());
	}

	bool Configuration::RequestForSmoothScrollingWarning()
	{
		if (!actualConfig->ReadBool(KeySmoothScrollingWarning, false))
		{
			actualConfig->Write(KeySmoothScrollingWarning, true);
			return true;
		}
		return false;
	}

}