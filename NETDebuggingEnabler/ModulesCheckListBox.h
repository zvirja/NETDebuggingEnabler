#pragma once
#include "baseset.h"
#include <wx/wx.h>
#include "WildcardFilter.h"
#include "OptimizationController.h"

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
			const wxString& name = wxListBoxNameStr);;
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

		void SetPathFilter(const wxString& value)
		{
			wildcardFilter.SetFilter(value);
			RefreshModules();
		}

	private:
		std::unique_ptr<std::vector<wxString>> modulesList;
		bool displayFullPath;
		WildcardFilter wildcardFilter;

		wxString ModulesCheckListBox::ApplyFilenameOnlyDecoration(const wxString& modulePath);
		std::vector<wxString> ApplyPathFilter(const std::vector < wxString> &modules);
		wxFrame* GetParentFrame(){ return static_cast<wxFrame*>(this->m_parent); }

		void OnModuleCheckedChanged(wxCommandEvent& event);
	};
}