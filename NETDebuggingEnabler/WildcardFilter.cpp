#include "baseset.h"
#include "WildcardFilter.h"

namespace Frames
{
	void WildcardFilter::SetFilter(const wxString& newFilter, int defaultWildcardPosition)
	{
		if (newFilter.length() == 0)
		{
			this->filter = wxEmptyString;
			return;
		}
		if (newFilter.StartsWith(L"*"))
		{
			if (newFilter.EndsWith(L"*"))
			{
				this->filter = newFilter.Mid(1, newFilter.length() - 2).Upper();
				this->matchMode = 0;
			}
			else
			{
				this->filter = newFilter.Mid(1, newFilter.length() - 1).Upper();
				this->matchMode = -1;
			}
		}
		else
		{
			if (newFilter.EndsWith(L"*"))
			{
				this->filter = newFilter.Mid(0, newFilter.length() - 1).Upper();
				this->matchMode = 1;
			}
			else
			{
				this->filter = newFilter.Upper();
				this->matchMode = defaultWildcardPosition;
			}
		}
	}

	bool WildcardFilter::MatchFilter(const wxString& valueToFit)
	{
		if (filter.length() == 0 || filter == wxEmptyString)
			return true;
		switch (this->matchMode)
		{
		case -1:
			return valueToFit.Upper().EndsWith(this->filter);
		case 0:
			return valueToFit.Upper().Contains(this->filter);
		case 1:
			return valueToFit.Upper().StartsWith(filter);
		default:
			return valueToFit.Upper() == this->filter;
		}
	}
}