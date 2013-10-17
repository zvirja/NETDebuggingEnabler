#include "baseset.h"
#include "ProcessInfo.h"

using namespace std;

namespace Frames
{
	ProcessInfo::ProcessInfo(const wxString& processName, int processId)
	{
		this->processName = shared_ptr<wxString>(new wxString(processName));
		this->processId = processId;
	}


	ProcessInfo::~ProcessInfo()
	{
	}

	wxString& ProcessInfo::GetName() const
	{
		return *this->processName;
	}

	int ProcessInfo::GetID() const
	{
		return this->processId;
	}

	wxString ProcessInfo::GetNameToDisplay()
	{
		if (nameHint.IsEmpty())
			return wxString::Format(L"%s (%d)", *processName, processId);
		else
			return wxString::Format(L"%s (%d) -- [%s]", *processName, processId, nameHint);

	}

}