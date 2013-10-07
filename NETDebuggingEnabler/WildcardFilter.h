#pragma once
#include "baseset.h"

namespace Frames
{
	class WildcardFilter
	{
	public:
		WildcardFilter() { 
			filter = wxEmptyString;
			matchMode = 0;
		}
		void SetFilter(const wxString& newFilter, int defaultWildcardPosition = 1);
		bool MatchFilter(const wxString& valueToFit);
	private:
		wxString filter;
		int matchMode;
	};
}