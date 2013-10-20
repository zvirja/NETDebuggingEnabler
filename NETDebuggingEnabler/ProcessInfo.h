#pragma once
#include <memory>
#include <wx/wx.h>
#include <wx/clntdata.h>

class ProcessInfo :public wxClientData
{
public:
	ProcessInfo(const wxString& processName, int processId);
	~ProcessInfo();
	wxString& GetName() const;
	int GetID() const;
	void SetNameHint(const wxString& hint) { nameHint = hint; }
	wxString& GetNameHint() { return nameHint; }
	wxString GetNameToDisplay();
private:
	std::shared_ptr<wxString> processName;
	wxString nameHint;
	int processId;
};
