#pragma once
#include <wx/frame.h>
#include <wx/event.h>
#include <wx/combobox.h>
#include <memory>
#include "ProcessManager.h"
#include <wx/textctrl.h>
#include <wx/checkbox.h>


namespace Frames
{

#define ID_PROCESSLIST wxID_HIGHEST+1
#define ID_PROCESSFILTERBOX wxID_HIGHEST+2
#define ID_PROCESSFILTERTEXT wxID_HIGHEST+3
#define ID_MODULESLISTBOX wxID_HIGHEST+4



	class MainFrame : public wxFrame
	{
	public:
		MainFrame();
		~MainFrame();
	private:
		static const int FrameWidth = 350;
		static const int FrameHeight = 500;
		wxComboBox* processesBox;
		wxCheckBox* filterCheckBox;
		wxTextCtrl* filterTextBox;
		wxCheckListBox* modulesListBox;
		std::unique_ptr<Managers::ProcessManager> processManager;

		void RefreshProcessList();
		std::vector<ProcessInfo> FilterProcesses(const std::vector < Frames::ProcessInfo >& processes);
		void FillModulesForProcess(const std::vector<wxString>& modules);
		void UpdateModulesForProcessInfo(const Frames::ProcessInfo& processInfo);


		DECLARE_EVENT_TABLE();
		void OnRefresh(wxCommandEvent& event);
		void OnProcessSelected(wxCommandEvent& event);
		void OnFilterEnabledChanged(wxCommandEvent& event);
		void OnFilterTextChanged(wxCommandEvent& event);
	};
}