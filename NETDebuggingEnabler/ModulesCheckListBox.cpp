#include "baseset.h"
#include "ModulesCheckListBox.h"
#include "ModuleCheckListBoxItem.h"
#include <wx\filename.h>

using namespace std;
namespace Frames
{

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

		for (wxString& module : filteredByPath)
		{
			auto decoratedModuleName = this->ApplyFilenameOnlyDecoration(module);
			this->Append(decoratedModuleName, new ModuleCheckListBoxItem(module));
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
		if (this->pathFilter == wxEmptyString)
			return modules;
		wxString filter = this->pathFilter;
		filter.Trim().MakeUpper();
		if (filter.length() == 0)
			return modules;
		vector<wxString> result;
		for (auto& modulePath : modules)
		{
			if (modulePath.Upper().StartsWith(filter))
				result.push_back(modulePath);
		}
		return result;
	}
}