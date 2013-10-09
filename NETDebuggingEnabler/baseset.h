#pragma once
#include <wx/wxprec.h>
#include <wx\msgdlg.h>
#include <wx\frame.h>
#include <wx\event.h>
#include <wx\combobox.h>
#include <wx\textctrl.h>
#include <wx\checkbox.h>
#include <wx\statline.h>
#include <wx\button.h>
#include <wx\listbox.h>
#include <wx\checklst.h>
#include <wx\wupdlock.h>

//STD
#include <vector>
#include <memory>
#include <map>
#include <set>


#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif

//#ifdef _DEBUG
//#include <crtdbg.h>
//#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
//#else
//#define DEBUG_NEW new
//#endif
//
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


//This solution
#include "unique_handle.h"
#include "StatusTextStackUpdater.h"
#include "ProcessManager.h"
