#include "baseset.h"
#include "ModulesCheckListBox.h"
#include "ModuleCheckListBoxItem.h"
#include <wx\filename.h>

using namespace std;
namespace Frames
{
	ModulesCheckListBox::ModulesCheckListBox(
		wxWindow *parent, wxWindowID winid, const wxPoint& pos, 
		const wxSize& size, const wxArrayString& choices, long style /*= 0*/, 
		const wxValidator& 	validator /*= wxDefaultValidator*/, const wxString& name /*= wxListBoxNameStr*/) : 
		wxCheckListBox(parent, winid, pos, size, choices, style, validator, name), displayFullPath(false), wildcardFilter()
	{
		this->modulesList = std::unique_ptr<std::vector<wxString>>(nullptr);
		this->Bind(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, &ModulesCheckListBox::OnModuleCheckedChanged, this);
	}

	ModulesCheckListBox::~ModulesCheckListBox()
	{
	}

	void ModulesCheckListBox::ReInitializeModules(const std::vector<wxString>& modules)
	{
		auto copy = new vector<wxString>(modules);
		modulesList.reset(copy);
		RefreshModules();
	}

	void ModulesCheckListBox::ClearModules()
	{
		modulesList.reset();
		RefreshModules();
	}

	void ModulesCheckListBox::RefreshModules()
	{
		StatusTextStackUpdater statusUpdater(GetParentFrame(), L"Updating modules list");
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

	wxString ModulesCheckListBox::ApplyFilenameOnlyDecoration(const wxString& modulePath)
	{
		if (displayFullPath)
			return modulePath;
		wxFileName modulePathFileName(modulePath);
		return wxString::Format(L"%s.%s", modulePathFileName.GetName(), modulePathFileName.GetExt());
	}
	std::vector<wxString> ModulesCheckListBox::ApplyPathFilter(const std::vector < wxString> &modules)
	{
		vector<wxString> result;
		for (auto& modulePath : modules)
		{
			if (wildcardFilter.MatchFilter(modulePath))
				result.push_back(modulePath);
		}
		return result;
	}

	void ModulesCheckListBox::OnModuleCheckedChanged(wxCommandEvent& event)
	{
		unsigned int index = event.GetSelection();
		auto item = static_cast<ModuleCheckListBoxItem*>(this->GetClientObject(index));
		bool newValue = this->IsChecked(index);
		item->OptimizationDisabled(newValue);
	}
}