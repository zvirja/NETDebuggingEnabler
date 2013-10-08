///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "noname.h"

///////////////////////////////////////////////////////////////////////////

MyFrame1::MyFrame1( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerMainVert;
	bSizerMainVert = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerFilter;
	bSizerFilter = new wxBoxSizer( wxHORIZONTAL );
	
	filterCheckBox = new wxCheckBox( this, ID_PROCESSFILTERBOX, wxT("Process name filter:"), wxDefaultPosition, wxDefaultSize, 0 );
	filterCheckBox->SetValue(true); 
	bSizerFilter->Add( filterCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxTOP, 5 );
	
	filterTextBox = new wxTextCtrl( this, ID_PROCESSFILTERTEXT, wxEmptyString, wxDefaultPosition, wxSize( 205,-1 ), 0 );
	bSizerFilter->Add( filterTextBox, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxTOP, 5 );
	
	
	bSizerMainVert->Add( bSizerFilter, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	wxBoxSizer* bSizerProsesses;
	bSizerProsesses = new wxBoxSizer( wxHORIZONTAL );
	
	processesBox = new wxComboBox( this, ID_PROCESSLIST, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY|wxCB_SORT ); 
	bSizerProsesses->Add( processesBox, 1, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	refreshButton = new wxButton( this, wxID_REFRESH, wxT("Refresh list"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerProsesses->Add( refreshButton, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5 );
	
	
	bSizerMainVert->Add( bSizerProsesses, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizerMainVert->Add( m_staticline1, 0, wxEXPAND|wxLEFT|wxRIGHT, 10 );
	
	wxBoxSizer* bSizerModulesControl;
	bSizerModulesControl = new wxBoxSizer( wxHORIZONTAL );
	
	modulesRefreshButton = new wxButton( this, ID_MODULES_REFRESHBUTTON, wxT("RM"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	modulesRefreshButton->SetToolTip( wxT("Reload modules") );
	
	bSizerModulesControl->Add( modulesRefreshButton, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );
	
	mudulesFullPathCheckBox = new wxCheckBox( this, ID_MODULES_DISPLAYFULLPATHBOX, wxT("Full paths"), wxDefaultPosition, wxDefaultSize, 0 );
	mudulesFullPathCheckBox->SetToolTip( wxT("Display full paths") );
	
	bSizerModulesControl->Add( mudulesFullPathCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );
	
	modulesFilterPathCheckBox = new wxCheckBox( this, ID_MODULES_APPLYPATHFILTERBOX, wxT("Path filter:"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerModulesControl->Add( modulesFilterPathCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 10 );
	
	modulesFilterTextCtrl = new wxTextCtrl( this, ID_MODULES_FILTERTEXT, wxT("*ASP.NET*"), wxDefaultPosition, wxDefaultSize, 0 );
	modulesFilterTextCtrl->Enable( false );
	modulesFilterTextCtrl->SetToolTip( wxT("Wildcard support: startsWith*,*endsWith,*contains*\nIf no wildcard present, check if starts with filter value.") );
	
	bSizerModulesControl->Add( modulesFilterTextCtrl, 1, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxTOP, 5 );
	
	
	bSizerMainVert->Add( bSizerModulesControl, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	wxBoxSizer* bSizerModules;
	bSizerModules = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString modulesListBoxChoices;
	modulesListBox = new wxCheckListBox( this, ID_MODULESLISTBOX, wxDefaultPosition, wxDefaultSize, modulesListBoxChoices, wxLB_SORT );
	bSizerModules->Add( modulesListBox, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	
	bSizerMainVert->Add( bSizerModules, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	
	this->SetSizer( bSizerMainVert );
	this->Layout();
	
	this->Centre( wxBOTH );
}

MyFrame1::~MyFrame1()
{
}
