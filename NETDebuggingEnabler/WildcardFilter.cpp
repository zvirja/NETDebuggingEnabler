#include "baseset.h"
#include "WildcardFilter.h"

void WildcardFilter::SetFilter(const wxString& newFilter, int defaultWildcardPosition)
{
	if (newFilter.length() == 0)
	{
		this->filter = wxEmptyString;
		return;
	}
	wxString newFilterChanged = newFilter;
	if (newFilterChanged.StartsWith(L"^"))
	{
		caseSensitive = true;
		newFilterChanged = newFilterChanged.Mid(1, newFilterChanged.length() - 1);
	}
	else
	{
		caseSensitive = false;
	}
	if (newFilterChanged.StartsWith(L"*"))
	{
		if (newFilterChanged.EndsWith(L"*"))
		{
			this->filter = GetUpperedValueIfNeed(newFilterChanged.Mid(1, newFilterChanged.length() - 2));
			this->matchMode = 0;
		}
		else
		{
			this->filter = GetUpperedValueIfNeed(newFilterChanged.Mid(1, newFilterChanged.length() - 1));
			this->matchMode = -1;
		}
	}
	else
	{
		if (newFilterChanged.EndsWith(L"*"))
		{
			this->filter = GetUpperedValueIfNeed(newFilterChanged.Mid(0, newFilterChanged.length() - 1));
			this->matchMode = 1;
		}
		else
		{
			this->filter = GetUpperedValueIfNeed(newFilterChanged);
			this->matchMode = defaultWildcardPosition;
		}
	}
}

bool WildcardFilter::MatchFilter(const wxString& valueToFit)
{
	if (filter.length() == 0 || filter == wxEmptyString)
		return true;
	wxString valueToFitUppered = GetUpperedValueIfNeed(valueToFit);
	switch (this->matchMode)
	{
	case -1:
		return valueToFitUppered.EndsWith(this->filter);
	case 0:
		return valueToFitUppered.Contains(this->filter);
	case 1:
		return valueToFitUppered.StartsWith(filter);
	default:
		return valueToFitUppered == this->filter;
	}
}