#pragma once
#include "baseset.h"

namespace UI
{
	class StatusTextStackUpdater
	{
	public:
		StatusTextStackUpdater(wxFrame* frame, const wxString& newStatus)
		{
			principal = frame;
			principal->PushStatusText(newStatus);
			alreadyPopped = false;
		}
		~StatusTextStackUpdater()
		{
			if (!alreadyPopped)
			{
				alreadyPopped = true;
				principal->PopStatusText();
			}
		};
	private:
		bool alreadyPopped;
		wxFrame* principal;
	};
}