#include "baseset.h"
#include "MainFrame.h"

using namespace std;
using namespace Managers;

namespace Frames
{
	MainFrame::MainFrame() :wxFrame(nullptr, wxID_ANY, wxT(".NET assembly debugging enabler"),
		wxDefaultPosition, wxSize(FrameWidth, FrameHeight),
		wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxNO_FULL_REPAINT_ON_RESIZE)
	{
		this->processManager = unique_ptr < ProcessManager>(new ProcessManager());

		//Frame Initialization

		SetIcon(wxICON(BUGICON2)); 

		this->SetSizeHints(FrameWidth, FrameHeight);
		this->BuildLayout();
		this->Centre(wxBOTH);
		this->SetBackgroundColour(wxColour(L"LightGray"));
		this->SetStatusBar(new wxStatusBar(this));
		this->SetStatusText("Ready");
		RefreshProcessList();
	}

	void MainFrame::BuildLayout()
	{
		wxBoxSizer* bSizerMainVert;
		bSizerMainVert = new wxBoxSizer(wxVERTICAL);

		wxBoxSizer* bSizerFilter;
		bSizerFilter = new wxBoxSizer(wxHORIZONTAL);

		filterCheckBox = new wxCheckBox(this, ID_PROCESSFILTERBOX, wxT("Process name filter:"), wxDefaultPosition, wxDefaultSize, 0);
		filterCheckBox->SetValue(true);
		bSizerFilter->Add(filterCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM | wxLEFT | wxTOP, 5);

		filterTextBox = new wxTextCtrl(this, ID_PROCESSFILTERTEXT, wxT("chrome.exe"), wxDefaultPosition, wxSize(205, -1), 0);
		bSizerFilter->Add(filterTextBox, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM | wxRIGHT | wxTOP, 5);


		bSizerMainVert->Add(bSizerFilter, 0, wxLEFT | wxRIGHT | wxTOP, 5);

		wxBoxSizer* bSizerProsesses;
		bSizerProsesses = new wxBoxSizer(wxHORIZONTAL);

		processesBox = new wxComboBox(this, ID_PROCESSLIST, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
		bSizerProsesses->Add(processesBox, 1, wxALIGN_CENTER_VERTICAL | wxBOTTOM | wxEXPAND | wxLEFT | wxRIGHT, 5);

		refreshButton = new wxButton(this, wxID_REFRESH, wxT("Refresh list"), wxDefaultPosition, wxDefaultSize, 0);
		bSizerProsesses->Add(refreshButton, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM | wxLEFT | wxRIGHT, 5);


		bSizerMainVert->Add(bSizerProsesses, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

		auto m_staticline1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
		bSizerMainVert->Add(m_staticline1, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

		wxBoxSizer* bSizerModulesControl;
		bSizerModulesControl = new wxBoxSizer(wxHORIZONTAL);

		mudulesFullPathCheckBox = new wxCheckBox(this, ID_MODULES_DISPLAYFULLPATHBOX, wxT("Full paths"), wxDefaultPosition, wxDefaultSize, 0);
		mudulesFullPathCheckBox->SetToolTip(wxT("Display full paths"));

		bSizerModulesControl->Add(mudulesFullPathCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

		modulesFilterPathCheckBox = new wxCheckBox(this, ID_MODULES_APPLYPATHFILTERBOX, wxT("Path filter:"), wxDefaultPosition, wxDefaultSize, 0);
		bSizerModulesControl->Add(modulesFilterPathCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

		modulesFilterTextCtrl = new wxTextCtrl(this, ID_MODULES_FILTERTEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		modulesFilterTextCtrl->Enable(false);

		bSizerModulesControl->Add(modulesFilterTextCtrl, 1, wxALIGN_CENTER_VERTICAL | wxBOTTOM | wxRIGHT | wxTOP, 5);

		bSizerMainVert->Add(bSizerModulesControl, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

		wxBoxSizer* bSizerModules;
		bSizerModules = new wxBoxSizer(wxHORIZONTAL);

		wxArrayString modulesListBoxChoices;
		modulesListBox = new ModulesCheckListBox(this, ID_MODULESLISTBOX, wxDefaultPosition, wxDefaultSize, modulesListBoxChoices, 0);
		bSizerModules->Add(modulesListBox, 1, wxBOTTOM | wxEXPAND | wxLEFT | wxRIGHT, 5);


		bSizerMainVert->Add(bSizerModules, 1, wxBOTTOM | wxEXPAND | wxLEFT | wxRIGHT, 5);


		this->SetSizer(bSizerMainVert);
		this->Layout();
	}

	MainFrame::~MainFrame()
	{
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
		processItems = FilterProcesses(processItems);
		for (auto iterator = processItems.begin(); iterator != processItems.end(); iterator++)
		{
			ProcessInfo& info = *iterator;
			processesBox->Append(wxString::Format(L"%s (%d)", info.GetName(), info.GetID()), new ProcessInfo(info));
		}
		if (processItems.size() > 0)
		{
			processesBox->SetSelection(0);
			ProcessInfo* processInfo = static_cast<ProcessInfo*>(processesBox->GetClientObject(0));
			UpdateModulesForProcessInfo(*processInfo);
		}

	}

	vector<ProcessInfo> MainFrame::FilterProcesses(const vector < ProcessInfo >& processes)
	{
		if (!filterCheckBox->IsChecked())
			return processes;
		auto textToFilter = filterTextBox->GetValue().Upper();
		vector<ProcessInfo> result;
		for (auto& process : processes)
		{
			if (process.GetName().Upper().Contains(textToFilter))
				result.push_back(process);
		}
		return move(result);
	}

	void MainFrame::UpdateModulesForProcessInfo(const Frames::ProcessInfo& processInfo)
	{
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
		EVT_BUTTON(wxID_REFRESH, MainFrame::OnRefresh)
		EVT_COMBOBOX(ID_PROCESSLIST, MainFrame::OnProcessSelected)
		EVT_CHECKBOX(ID_PROCESSFILTERBOX, MainFrame::OnFilterEnabledChanged)
		EVT_TEXT(ID_PROCESSFILTERTEXT, MainFrame::OnFilterTextChanged)
		EVT_CHECKBOX(ID_MODULES_DISPLAYFULLPATHBOX, MainFrame::OnDisplayFullPathChanged)
		EVT_CHECKBOX(ID_MODULES_APPLYPATHFILTERBOX, MainFrame::OnModulePathFilterEnabledChanged)
		EVT_TEXT(ID_MODULES_FILTERTEXT, MainFrame::OnModulePathFilterTextChanged)
	END_EVENT_TABLE()

	void MainFrame::OnRefresh(wxCommandEvent& event)
	{
		RefreshProcessList();
	}

	void MainFrame::OnProcessSelected(wxCommandEvent& event)
	{
		ProcessInfo* obj = static_cast<ProcessInfo*>(event.GetClientObject());
		UpdateModulesForProcessInfo(*obj);
	}

	void MainFrame::OnFilterEnabledChanged(wxCommandEvent& event)
	{
		OnRefresh(event);
		filterTextBox->Enable(filterCheckBox->IsChecked());
	}

	void MainFrame::OnFilterTextChanged(wxCommandEvent& event)
	{
		OnRefresh(event);
	}

	void MainFrame::OnDisplayFullPathChanged(wxCommandEvent& event)
	{
		auto displayFullPathBox = static_cast<wxCheckBox*>(event.GetEventObject());
		this->modulesListBox->SetDispayFullPath(displayFullPathBox->IsChecked());
	}

	void MainFrame::OnModulePathFilterEnabledChanged(wxCommandEvent& event)
	{
		auto enablePathFilter = static_cast<wxCheckBox*>(event.GetEventObject());
		if (enablePathFilter->IsChecked())
		{
			this->modulesListBox->SetPathFilter(this->modulesFilterTextCtrl->GetValue());
			this->modulesFilterTextCtrl->Enable(true);
		}
		else
		{
			this->modulesListBox->SetPathFilter(wxEmptyString);
			this->modulesFilterTextCtrl->Enable(false);

		}
	}

	void MainFrame::OnModulePathFilterTextChanged(wxCommandEvent& event)
	{
		this->modulesListBox->SetPathFilter(this->modulesFilterTextCtrl->GetValue());
	}

}