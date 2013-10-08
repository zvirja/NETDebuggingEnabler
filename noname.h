///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __NONAME_H__
#define __NONAME_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/checklst.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_PROCESSFILTERBOX 1000
#define ID_PROCESSFILTERTEXT 1001
#define ID_PROCESSLIST 1002
#define ID_MODULES_REFRESHBUTTON 1003
#define ID_MODULES_DISPLAYFULLPATHBOX 1004
#define ID_MODULES_APPLYPATHFILTERBOX 1005
#define ID_MODULES_FILTERTEXT 1006
#define ID_MODULESLISTBOX 1007

///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame1
///////////////////////////////////////////////////////////////////////////////
class MyFrame1 : public wxFrame 
{
	private:
	
	protected:
		wxCheckBox* filterCheckBox;
		wxTextCtrl* filterTextBox;
		wxComboBox* processesBox;
		wxButton* refreshButton;
		wxStaticLine* m_staticline1;
		wxButton* modulesRefreshButton;
		wxCheckBox* mudulesFullPathCheckBox;
		wxCheckBox* modulesFilterPathCheckBox;
		wxTextCtrl* modulesFilterTextCtrl;
		wxCheckListBox* modulesListBox;
	
	public:
		
		MyFrame1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MyFrame1();
	
};

#endif //__NONAME_H__
