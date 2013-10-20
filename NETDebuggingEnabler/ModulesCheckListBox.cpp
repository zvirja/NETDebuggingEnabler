#include "baseset.h"
#include "ModulesCheckListBox.h"
#include "ModuleCheckListBoxItem.h"
#include <wx\filename.h>

using namespace std;
namespace UI
{
	ModulesCheckListBox::ModulesCheckListBox(
		wxWindow *parent, wxWindowID winid, const wxPoint& pos,
		const wxSize& size, const wxArrayString& choices, long style /*= 0*/,
		const wxValidator& 	validator /*= wxDefaultValidator*/, const wxString& name /*= wxListBoxNameStr*/) :
		wxCheckListBox(parent, winid, pos, size, choices, style, validator, name), displayFullPath(false), wildcardFilters()
	{
		this->modulesList = std::unique_ptr<std::vector<wxString>>(nullptr);
		this->Bind(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, &ModulesCheckListBox::OnModuleCheckedChanged, this);
	}

	ModulesCheckListBox::~ModulesCheckListBox()
	{
	}

	void ModulesCheckListBox::ReInitializeModules(const std::vector<wxString>& modules)
	{
		modulesList.reset(new vector<wxString>(modules));
		RefreshModules();
	}

	void ModulesCheckListBox::ClearModules()
	{
		modulesList.reset();
		RefreshModules();
	}

	void ModulesCheckListBox::RefreshModules()
	{
		StatusTextStackUpdater statusUpdater(GetParentFrame(), L"Updating modules list...");
		wxWindowUpdateLocker uiLocker(this);
		this->Clear();
		if (modulesList.get() == nullptr)
			return;
		auto filteredByPath = this->ApplyPathFilter(*modulesList);
		if (filteredByPath.size() == 0)
			return;

		int counter = 0;
		for (wxString& module : filteredByPath)
		{
			auto decoratedModuleName = this->ApplyFilenameOnlyDecoration(module);
			auto clientData = new ModuleCheckListBoxItem(module);
			this->Append(decoratedModuleName, clientData);
		}
		for (unsigned int i = 0; i < this->GetCount(); i++)
		{
			this->Check(i, static_cast<ModuleCheckListBoxItem*>(this->GetClientObject(i))->IsOptimizationDisabled());
		}
	}


	void ModulesCheckListBox::SetPathFilterValue(const wxString& value)
	{
		wildcardFilters.clear();
		if (value.length() != 0)
		{
			auto resultOfSplit = wxSplit(value, L'|');
			for (auto& individualFilter : resultOfSplit)
			{
				if (individualFilter.length() == 0)
					continue;
				WildcardFilter filter(individualFilter);
				wildcardFilters.push_back(filter);
			}
		}
		RefreshModules();
	}


	wxString ModulesCheckListBox::ApplyFilenameOnlyDecoration(const wxString& modulePath)
	{
		if (displayFullPath)
			return modulePath;
		wxFileName modulePathFileName(modulePath);
		return wxString::Format(L"%s.%s", modulePathFileName.GetName(), modulePathFileName.GetExt());
	}
	std::vector<wxString> ModulesCheckListBox::ApplyPathFilter(const std::vector < wxString> &modules)
	{

		if (!enablePathFilter || wildcardFilters.size() == 0)
			return modules;
		vector<wxString> result;
		for (auto& modulePath : modules)
		{
			if (MatchWildcardFilters(modulePath))
				result.push_back(modulePath);
		}
		return result;
	}

	void ModulesCheckListBox::OnModuleCheckedChanged(wxCommandEvent& event)
	{
		unsigned int index = event.GetSelection();
		auto item = static_cast<ModuleCheckListBoxItem*>(this->GetClientObject(index));
		bool newValue = this->IsChecked(index);
		if (!item->OptimizationDisabled(newValue))
		{
			this->Check(index, !newValue);
			wxMessageBox(L"Unable to modify related *.ini file. \nPlease ensure that file is editable (e.g. isn't a system file, used by another process).", L".NET Debugging enabler", wxICON_ERROR | wxOK, this->GetParent());
		}
	}

	bool ModulesCheckListBox::MatchWildcardFilters(const wxString& modulePath)
	{
		if (wildcardFilters.size() == 0)
			return true;
		for (auto& filter : wildcardFilters)
		{
			if (!filter.MatchFilter(modulePath))
				return false;
		}
		return true;
	}

}