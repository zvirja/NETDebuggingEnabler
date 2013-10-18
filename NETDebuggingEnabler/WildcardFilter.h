#pragma once
#include "baseset.h"

namespace Frames
{
	class WildcardFilter
	{
	public:
		WildcardFilter(const wxString& filterValue): caseSensitive(false) {
			SetFilter(filterValue);
		}
		void SetFilter(const wxString& newFilter, int defaultWildcardPosition = 1);
		bool MatchFilter(const wxString& valueToFit);
	private:
		wxString filter;
		int matchMode;
		bool caseSensitive;
		wxString GetUpperedValueIfNeed(const wxString& input) { return caseSensitive ? input : input.Upper(); }
	};
}