#pragma once
#include <memory>
#include <wx/wx.h>
#include <wx/clntdata.h>

namespace Frames
{
	class ProcessInfo :public wxClientData
	{
	public:
		ProcessInfo(const wxString& processName, int processId);
		~ProcessInfo();
		wxString& GetName() const;
		int GetID() const;
	private:
		std::shared_ptr<wxString> processName;
		int processId;
	};
}
