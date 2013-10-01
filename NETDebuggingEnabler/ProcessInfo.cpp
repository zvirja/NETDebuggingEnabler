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
}