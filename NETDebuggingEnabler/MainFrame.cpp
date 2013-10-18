#include "baseset.h"
#include "MainFrame.h"
#include "NETDebuggingEnablerApp.h"
#include <wx/imagpng.h>

using namespace std;
using namespace Managers;

namespace Frames
{
	MainFrame::MainFrame() : isReady(false), wxFrame(nullptr, wxID_ANY, wxT(".NET assembly debugging enabler"),
		wxDefaultPosition, wxSize(FrameWidth, FrameHeight),
		wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxNO_FULL_REPAINT_ON_RESIZE)
	{
		this->processManager = unique_ptr < ProcessManager>(new ProcessManager());

		//Register PNG image handler
		wxImage::AddHandler(new wxPNGHandler);

		//Frame Initialization

		SetIcon(wxICON(BUGICON2));

		this->SetSizeHints(FrameWidth, FrameHeight);
		this->BuildLayout();
		this->SetBackgroundColour(wxColour(L"LightGray"));
		this->SetStatusBar(new wxStatusBar(this));
		this->SetStatusText(L"Ready");

		this->SetPosition(NETDebuggingEnablerApp::AppConfig->GetLastPosition());
		this->SetSize(NETDebuggingEnablerApp::AppConfig->GetLastSize());

		RestoreValuesFromConfig();
		isReady = true;
		RefreshProcessList();

		processesBox->SetFocus();
	}

	void MainFrame::BuildLayout()
	{
		wxBoxSizer* bSizerMainVert;
		bSizerMainVert = new wxBoxSizer(wxVERTICAL);

		wxBoxSizer* bSizerFilter;
		bSizerFilter = new wxBoxSizer(wxHORIZONTAL);

		onlyNETBox = new wxCheckBox(this, ID_PROCESSLIST_ONLYNETBOX, wxT("Only .NET"), wxDefaultPosition, wxDefaultSize, 0);
		onlyNETBox->SetToolTip(wxT("Specifies to display only .NET related processes.\nProcess is considered as .NET related if it refers to the Microsoft.NET\\Framework* stuff.\nIT'S RELATIVELY SLOW AND EXPENSIVE, so state isn't saved across sessions to not slow startup."));

		bSizerFilter->Add(onlyNETBox, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM | wxLEFT | wxTOP, 5);

		processNameFilterCheckBox = new wxCheckBox(this, ID_PROCESSFILTERBOX, wxT("Name filter:"), wxDefaultPosition, wxDefaultSize, 0);
		bSizerFilter->Add(processNameFilterCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM | wxLEFT | wxTOP, 5);

		processNameFilterTextBox = new wxTextCtrl(this, ID_PROCESSFILTERTEXT, wxEmptyString, wxDefaultPosition, wxSize(205, -1), 0);
		processNameFilterTextBox->Enable(false);
		processNameFilterTextBox->SetToolTip(wxT("Value is used as *contains* filter."));

		bSizerFilter->Add(processNameFilterTextBox, 1, wxALIGN_CENTER_VERTICAL | wxBOTTOM | wxRIGHT | wxTOP, 5);


		bSizerMainVert->Add(bSizerFilter, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 5);

		wxBoxSizer* bSizerProsesses;
		bSizerProsesses = new wxBoxSizer(wxHORIZONTAL);

		processesBox = new wxComboBox(this, ID_PROCESSLIST, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY | wxCB_SORT);
		bSizerProsesses->Add(processesBox, 1, wxALIGN_CENTER_VERTICAL | wxBOTTOM | wxEXPAND | wxLEFT | wxRIGHT, 5);

		refreshButton = new wxButton(this, wxID_REFRESH, wxT("Refresh list"), wxDefaultPosition, wxDefaultSize, 0);
		bSizerProsesses->Add(refreshButton, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM | wxLEFT | wxRIGHT, 5);


		bSizerMainVert->Add(bSizerProsesses, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

		m_staticline1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
		bSizerMainVert->Add(m_staticline1, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

		wxBoxSizer* bSizerModulesControl;
		bSizerModulesControl = new wxBoxSizer(wxHORIZONTAL);

		modulesRefreshButton = new wxBitmapButton(this, ID_MODULES_REFRESHBUTTON, wxBITMAP_PNG(REFRESH_PNG2), wxDefaultPosition, wxSize(30, 30), wxBU_AUTODRAW);
		modulesRefreshButton->SetToolTip(wxT("Reload modules for selected process"));

		bSizerModulesControl->Add(modulesRefreshButton, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

		modulesFullPathCheckBox = new wxCheckBox(this, ID_MODULES_DISPLAYFULLPATHBOX, wxT("Full paths"), wxDefaultPosition, wxDefaultSize, 0);
		modulesFullPathCheckBox->SetToolTip(wxT("Display full paths"));

		bSizerModulesControl->Add(modulesFullPathCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

		modulesFilterPathCheckBox = new wxCheckBox(this, ID_MODULES_APPLYPATHFILTERBOX, wxT("Path filter:"), wxDefaultPosition, wxDefaultSize, 0);
		bSizerModulesControl->Add(modulesFilterPathCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

		modulesFilterTextCtrl = new wxTextCtrl(this, ID_MODULES_FILTERTEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		modulesFilterTextCtrl->Enable(false);
		modulesFilterTextCtrl->SetToolTip(wxT("Wildcard support: startsWith*,*endsWith,*contains*\nUse ^ prefix to enable case sensitive match.\nUse pipe | to specify a few rules to match (e.g. *a*|^*B)\nIf no wildcard present, check if starts with filter value."));

		bSizerModulesControl->Add(modulesFilterTextCtrl, 1, wxALIGN_CENTER_VERTICAL | wxBOTTOM | wxRIGHT | wxTOP, 5);


		bSizerMainVert->Add(bSizerModulesControl, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

		wxBoxSizer* bSizerModules;
		bSizerModules = new wxBoxSizer(wxHORIZONTAL);

		wxArrayString modulesListBoxChoices;
		modulesListBox = new ModulesCheckListBox(this, ID_MODULESLISTBOX, wxDefaultPosition, wxDefaultSize, modulesListBoxChoices, wxLB_SORT);
		bSizerModules->Add(modulesListBox, 1, wxBOTTOM | wxEXPAND | wxLEFT | wxRIGHT, 5);


		bSizerMainVert->Add(bSizerModules, 1, wxBOTTOM | wxEXPAND | wxLEFT | wxRIGHT, 5);

		this->SetSizer(bSizerMainVert);
		this->Layout();
	}

	MainFrame::~MainFrame()
	{
	}


	void MainFrame::RestoreValuesFromConfig()
	{
		Configuration* appConfig = NETDebuggingEnablerApp::AppConfig;
		//Don't enable it for performance reason
		//onlyNETBox->SetValue(appConfig->GetOnlyNetProcesses());
		processNameFilterCheckBox->SetValue(appConfig->GetEnableProcessNameFilter());
		processNameFilterTextBox->Enable(processNameFilterCheckBox->IsChecked());
		processNameFilterTextBox->SetValue(appConfig->GetProcessNameFilter());


		modulesFullPathCheckBox->SetValue(appConfig->GetDisplayFullModulePaths());
		modulesFilterTextCtrl->SetValue(appConfig->GetModulePathFilter());
		modulesFilterPathCheckBox->SetValue(appConfig->GetEnableModulePathFilter());
		modulesFilterTextCtrl->Enable(modulesFilterPathCheckBox->IsChecked());

		modulesListBox->SetPathFilter(modulesFilterPathCheckBox->IsChecked() ? appConfig->GetModulePathFilter() : wxEmptyString);
		modulesListBox->SetDispayFullPath(modulesFullPathCheckBox->IsChecked());
	}


	void MainFrame::RefreshProcessList()
	{
		StatusTextStackUpdater statusUpdater(this, L"Process list refreshing...");
		wxWindowUpdateLocker thisUpdateLocker(processesBox);
		processesBox->Clear();
		modulesListBox->Clear();
		vector<ProcessInfo> processItems = processManager->GetProcessesList();
		if (processItems.size() == 0)
		{
			return;
		}
		processItems = FilterOnlyNETProcesses(FilterProcesses(processItems));
		for (auto iterator = processItems.begin(); iterator != processItems.end(); iterator++)
		{
			ProcessInfo& info = *iterator;
			processesBox->Append(info.GetNameToDisplay(), new ProcessInfo(info));
		}
		if (processItems.size() > 0)
		{
			processesBox->SetSelection(0);
			ProcessInfo* processInfo = static_cast<ProcessInfo*>(processesBox->GetClientObject(0));
			UpdateModulesForProcessInfo(*processInfo);
		}
		else
		{
			//If we don't perform this, the modules list will contain old entries.
			modulesListBox->ClearModules();
		}

	}

	vector<ProcessInfo> MainFrame::FilterProcesses(const vector < ProcessInfo >& processes)
	{
		if (!processNameFilterCheckBox->IsChecked())
			return processes;
		auto textToFilter = processNameFilterTextBox->GetValue().Upper();
		vector<ProcessInfo> result;
		for (auto& process : processes)
		{
			if (process.GetName().Upper().Contains(textToFilter))
				result.push_back(process);
		}
		return move(result);
	}

	vector<ProcessInfo> MainFrame::FilterOnlyNETProcesses(const vector < ProcessInfo >& processes)
	{
		if (!onlyNETBox->IsChecked())
			return processes;
		vector<ProcessInfo> result;
		DWORD resultDW;
		for (auto& process : processes)
		{
			vector<wxString> modulesForProc = processManager->GetModulesForProcessId(process.GetID(), resultDW, true);
			for (auto& modulePath : modulesForProc)
			{
				if (modulePath.Upper().Contains(L"MICROSOFT.NET\\FRAMEWORK"))
				{
					result.push_back(process);
					break;;
				}
			}
		}
		return move(result);
	}

	void MainFrame::UpdateModulesForProcessInfo(const Frames::ProcessInfo& processInfo)
	{
		StatusTextStackUpdater statusUpdater(this, L"Updating modules list...");
		DWORD result = 0;
		vector<wxString> modules = processManager->GetModulesForProcessId(processInfo.GetID(), result);
		if (result != ERROR_SUCCESS)
		{
			modulesListBox->ClearModules();

			//TCHAR   lpBuffer[256] = L"?";
			//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,                 // It´s a system error
			//	NULL,                                      // No string to be formatted needed
			//	result,                               // Hey Windows: Please explain this error!
			//	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // Do it in the standard language
			//	lpBuffer,              // Put the message here
			//	sizeof(lpBuffer),                     // Number of bytes to store the message
			//	NULL);
			//wxMessageBox(wxString::Format(L"Unable to read modules: %s", lpBuffer));
		}
		else
		{
			modulesListBox->ReInitializeModules(modules);
		}
	}


	BEGIN_EVENT_TABLE(MainFrame, wxFrame)
		EVT_CLOSE(MainFrame::OnClose)
		EVT_BUTTON(wxID_REFRESH, MainFrame::OnRefresh)
		EVT_COMBOBOX(ID_PROCESSLIST, MainFrame::OnProcessSelected)
		EVT_CHECKBOX(ID_PROCESSLIST_ONLYNETBOX, MainFrame::OnOnlyNETChanged)
		EVT_CHECKBOX(ID_PROCESSFILTERBOX, MainFrame::OnProcessNameFilterEnabledChanged)
		EVT_TEXT(ID_PROCESSFILTERTEXT, MainFrame::OnProcessNameFilterTextChanged)
		EVT_CHECKBOX(ID_MODULES_DISPLAYFULLPATHBOX, MainFrame::OnDisplayFullPathChanged)
		EVT_CHECKBOX(ID_MODULES_APPLYPATHFILTERBOX, MainFrame::OnModulePathFilterEnabledChanged)
		EVT_TEXT(ID_MODULES_FILTERTEXT, MainFrame::OnModulePathFilterTextChanged)
		EVT_BUTTON(ID_MODULES_REFRESHBUTTON, MainFrame::OnModulesReload)
		END_EVENT_TABLE()


		void	MainFrame::OnClose(wxCloseEvent& event)
		{
			if (!this->IsMaximized())
			{
				NETDebuggingEnablerApp::AppConfig->SetLastPosition(this->GetPosition());
				NETDebuggingEnablerApp::AppConfig->SaveLastSize(this->GetSize());
			}
			event.Skip();
		}


	void MainFrame::OnRefresh(wxCommandEvent& event)
	{
		if (!isReady)
			return;
		RefreshProcessList();
	}

	void MainFrame::OnProcessSelected(wxCommandEvent& event)
	{
		ProcessInfo* obj = static_cast<ProcessInfo*>(event.GetClientObject());
		UpdateModulesForProcessInfo(*obj);
	}

	void MainFrame::OnProcessNameFilterEnabledChanged(wxCommandEvent& event)
	{
		OnRefresh(event);
		processNameFilterTextBox->Enable(processNameFilterCheckBox->IsChecked());
		NETDebuggingEnablerApp::AppConfig->SetEnableProcessNameFilter(processNameFilterCheckBox->IsChecked());
	}

	void MainFrame::OnProcessNameFilterTextChanged(wxCommandEvent& event)
	{
		OnRefresh(event);
		NETDebuggingEnablerApp::AppConfig->SetProcessNameFilter(processNameFilterTextBox->GetValue());
	}


	void MainFrame::OnOnlyNETChanged(wxCommandEvent& event)
	{
		OnRefresh(event);
		//Isn't used
		//NETDebuggingEnablerApp::AppConfig->SetOnlyNetProcesses(onlyNETBox->IsChecked());
	}

	void MainFrame::OnDisplayFullPathChanged(wxCommandEvent& event)
	{
		auto displayFullPathBox = static_cast<wxCheckBox*>(event.GetEventObject());
		this->modulesListBox->SetDispayFullPath(displayFullPathBox->IsChecked());
		NETDebuggingEnablerApp::AppConfig->SetDisplayFullModulePaths(displayFullPathBox->IsChecked());
	}

	void MainFrame::OnModulePathFilterEnabledChanged(wxCommandEvent& event)
	{
		auto enablePathFilter = static_cast<wxCheckBox*>(event.GetEventObject());
		if (enablePathFilter->IsChecked())
		{
			this->modulesListBox->SetPathFilter(this->modulesFilterTextCtrl->GetValue());
		}
		else
		{
			this->modulesListBox->SetPathFilter(wxEmptyString);
		}
		this->modulesFilterTextCtrl->Enable(enablePathFilter->IsChecked());
		NETDebuggingEnablerApp::AppConfig->SetEnableModulePathFilter(enablePathFilter->IsChecked());
	}

	void MainFrame::OnModulePathFilterTextChanged(wxCommandEvent& event)
	{
		this->modulesListBox->SetPathFilter(this->modulesFilterTextCtrl->GetValue());
		NETDebuggingEnablerApp::AppConfig->SetModulePathFilter(modulesFilterTextCtrl->GetValue());
	}

	void MainFrame::OnModulesReload(wxCommandEvent& event)
	{
		auto selectedId = processesBox->GetCurrentSelection();
		if (selectedId < 0)
			return;
		auto procInfo = static_cast<ProcessInfo*>(processesBox->GetClientObject(selectedId));
		if (procInfo != nullptr)
			this->UpdateModulesForProcessInfo(*procInfo);
	}

}