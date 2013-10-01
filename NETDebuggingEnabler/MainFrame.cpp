#include "MainFrame.h"
#include <memory>
#include <wx\button.h>
#include <wx\listbox.h>
#include <wx\checklst.h>
#include <wx/wupdlock.h>
#include <wx\event.h>
#include <vector>
#include <wx\string.h>

using namespace std;
using namespace Managers;

namespace Frames
{
	MainFrame::MainFrame() :wxFrame(nullptr, wxID_ANY, wxT(".NET assembly debugging enabler"),
		wxDefaultPosition, wxSize(FrameWidth, FrameHeight),
		wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxNO_FULL_REPAINT_ON_RESIZE)
	{
		this->processManager = unique_ptr < ProcessManager>(new ProcessManager());

//Frame Initialization

		SetIcon(wxICON(BUGICON2));
		int rectW, rectH;

		this->GetClientSize(&rectW, &rectH);

		filterCheckBox = new wxCheckBox(this, ID_PROCESSFILTERBOX, "Filter by name", wxPoint(10, 8));
		filterCheckBox->SetValue(true);
		auto filterCheckBoxSize = filterCheckBox->GetSize();
		auto filterCheckBoxPos = filterCheckBox->GetPosition();

		filterTextBox = new wxTextCtrl(this, ID_PROCESSFILTERTEXT, wxT("chrome.exe"), 
			wxPoint(filterCheckBoxPos.x + filterCheckBoxSize.GetX() + 5, 6), wxSize(150, 20));


		processesBox = new wxComboBox(this, ID_PROCESSLIST, wxT(""), 
			wxPoint(10, filterCheckBoxPos.y + filterCheckBoxSize.GetY() + 10), wxSize(220, 25), wxArrayString(), wxCB_SORT);


		auto processesBoxSize = processesBox->GetSize();
		auto processesBoxPos = processesBox->GetPosition();

		wxButton* refreshButton = new wxButton(this, wxID_REFRESH, wxT("Refresh list"), 
			wxPoint(processesBoxPos.x + processesBoxSize.GetX() + 5, processesBoxPos.y - 2), wxSize(85, 27));

		modulesListBox = new wxCheckListBox(this, ID_MODULESLISTBOX,
			wxPoint(10, processesBoxPos.y + processesBoxSize.GetY() + 10),
			wxSize(rectW - 20, rectH - processesBoxPos.y - processesBoxSize.GetY() - 40),
			wxArrayString());

		this->SetBackgroundColour(wxColour(wxT("LightGray")));

		this->SetStatusBar(new wxStatusBar(this));
		
		this->SetStatusText("Ready");
		RefreshProcessList();
	}

	MainFrame::~MainFrame()
	{
	}

	void MainFrame::RefreshProcessList()
	{
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
			processesBox->Append(wxString::Format("%s (%d)", info.GetName(), info.GetID()), new ProcessInfo(info));
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
		for (auto iterator = processes.begin(); iterator != processes.end();++iterator)
		{
			if (iterator->GetName().Upper().Contains(textToFilter))
				result.push_back(*iterator);
		}
		return result;
	}

	void MainFrame::UpdateModulesForProcessInfo(const Frames::ProcessInfo& processInfo)
	{
		DWORD result = 0;
		vector<wxString> modules = processManager->GetModulesForProcessId(processInfo.GetID(), result);
		if (result != ERROR_SUCCESS)
		{
			TCHAR   lpBuffer[256] = _T("?");
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,                 // It´s a system error
				NULL,                                      // No string to be formatted needed
				result,                               // Hey Windows: Please explain this error!
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // Do it in the standard language
				lpBuffer,              // Put the message here
				sizeof(lpBuffer),                     // Number of bytes to store the message
				NULL);

				wxMessageBox(wxString::Format(L"Unable to read modules: %s", lpBuffer));
		}
		else
		{
			FillModulesForProcess(modules);
		}
	}
	
	void MainFrame::FillModulesForProcess(const vector<wxString>& modules)
	{
		this->PushStatusText(wxT("Fetching module list"));
		wxWindowUpdateLocker uiLocker(this);
		modulesListBox->Clear();
		for (auto iterator = modules.begin(); iterator != modules.end(); iterator++)
		{
			modulesListBox->Append(*iterator);
		}
		this->PopStatusText();
	}
	

	BEGIN_EVENT_TABLE(MainFrame, wxFrame)
		EVT_BUTTON(wxID_REFRESH, MainFrame::OnRefresh)
		EVT_COMBOBOX(ID_PROCESSLIST, MainFrame::OnProcessSelected)
		EVT_CHECKBOX(ID_PROCESSFILTERBOX, MainFrame::OnFilterEnabledChanged)
		EVT_TEXT(ID_PROCESSFILTERTEXT, MainFrame::OnFilterTextChanged)
	END_EVENT_TABLE()

	void MainFrame::OnRefresh(wxCommandEvent& event)
	{
		this->PushStatusText("Process list refreshing...");
		wxWindowUpdateLocker thisUpdateLocker(this);
		RefreshProcessList();
		this->PopStatusText();
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



}