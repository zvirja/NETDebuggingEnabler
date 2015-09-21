# NETDebuggingEnabler
## Overview

Tool for .NET assembly optimization disabling. To find more details how it works refer to the following article: [https://msdn.microsoft.com/en-us/library/9dd8z24x.aspx](https://msdn.microsoft.com/en-us/library/9dd8z24x.aspx).

Tool provides GUI and doesn't have any depencency.

## Main tool features:

* Full support for both x86 (WOW64) and x64 processes.
* Processes name filter (e.g. display only w3wp processes).
* Modules path filter (e.g. display only user assemblies for w3wp processes).
* Hints for w3wp processes (it displays application pool name, which is enough to choose the proper w3wp process).
* UI state saving across sessions (position, size, checkbox/textbox values).
