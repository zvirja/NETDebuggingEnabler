#pragma once
#include "baseset.h"
#include "ModulesCheckListBox.h"



namespace Frames
{

#define ID_PROCESSLIST wxID_HIGHEST+1
#define ID_PROCESSFILTERBOX wxID_HIGHEST+2
#define ID_PROCESSFILTERTEXT wxID_HIGHEST+3
#define ID_MODULESLISTBOX wxID_HIGHEST+4
#define ID_MODULES_DISPLAYFULLPATHBOX wxID_HIGHEST+5
#define ID_MODULES_APPLYPATHFILTERBOX wxID_HIGHEST+6
#define ID_MODULES_FILTERTEXT wxID_HIGHEST+7
#define ID_MODULES_REFRESHBUTTON wxID_HIGHEST+8
#define ID_PROCESSLIST_ONLYNETBOX wxID_HIGHEST+9



	class MainFrame : public wxFrame
	{
	public:
		MainFrame();
		~MainFrame();
	private:
		static const int FrameWidth = 350;
		static const int FrameHeight = 500;
		bool isReady = false;
		wxCheckBox* onlyNETBox;
		wxCheckBox* processNameFilterCheckBox;
		wxTextCtrl* processNameFilterTextBox;
		wxComboBox* processesBox;
		wxButton* refreshButton;
		wxCheckBox* modulesFullPathCheckBox;
		wxCheckBox* modulesFilterPathCheckBox;
		wxTextCtrl* modulesFilterTextCtrl;
		ModulesCheckListBox* modulesListBox;
		wxButton* modulesRefreshButton;
		wxStaticLine* m_staticline1;

		std::unique_ptr<Managers::ProcessManager> processManager;

		void BuildLayout();
		void RefreshProcessList();
		std::vector<ProcessInfo> FilterProcesses(const std::vector < Frames::ProcessInfo >& processes);
		std::vector<ProcessInfo> FilterOnlyNETProcesses(const std::vector < Frames::ProcessInfo >& processes);
		void FillModulesForProcess(const std::vector<wxString>& modules);
		void UpdateModulesForProcessInfo(const Frames::ProcessInfo& processInfo);
		void RestoreValuesFromConfig();


		DECLARE_EVENT_TABLE();
		void OnRefresh(wxCommandEvent& event);
		void OnProcessSelected(wxCommandEvent& event);
		void OnOnlyNETChanged(wxCommandEvent& event);
		void OnProcessNameFilterEnabledChanged(wxCommandEvent& event);
		void OnProcessNameFilterTextChanged(wxCommandEvent& event);

		void OnDisplayFullPathChanged(wxCommandEvent& event);
		void OnModulePathFilterEnabledChanged(wxCommandEvent& event);
		void OnModulePathFilterTextChanged(wxCommandEvent& event);
		void OnModulesReload(wxCommandEvent& event);
	};
}