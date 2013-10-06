#pragma once
#include "baseset.h"
#include <wx/wx.h>

namespace Frames
{

	class ModulesCheckListBox : public wxCheckListBox
	{
	public:
		ModulesCheckListBox(wxWindow *parent, wxWindowID winid,
			const wxPoint& pos,
			const wxSize& size,
			const wxArrayString& choices,
			long style = 0,
			const wxValidator& validator = wxDefaultValidator,
			const wxString& name = wxListBoxNameStr) : wxCheckListBox(parent, winid, pos, size, choices, style, validator, name)
		{
			this->modulesList = std::unique_ptr<std::vector<wxString>>(nullptr);
			this->displayFullPath = false;
			this->pathFilter = wxEmptyString;
		};
		virtual ~ModulesCheckListBox();

		void ReInitializeModules(const std::vector<wxString>& modules);
		void ClearModules();
		void RefreshModules();

		bool GetDispayFullPath() { return displayFullPath; }
		void SetDispayFullPath(bool value)
		{
			displayFullPath = value;
			RefreshModules();
		}

		wxString& GetPathFilter() { return pathFilter; }
		void SetPathFilter(const wxString& value)
		{
			pathFilter = value;
			RefreshModules();
		}

	private:
		std::unique_ptr<std::vector<wxString>> modulesList;
		bool displayFullPath;
		wxString pathFilter;

		wxString ModulesCheckListBox::ApplyFilenameOnlyDecoration(const wxString& modulePath);
		std::vector<wxString> ApplyPathFilter(const std::vector < wxString> &modules);
		wxFrame* GetParentFrame(){ return static_cast<wxFrame*>(this->m_parent); }
	};
}