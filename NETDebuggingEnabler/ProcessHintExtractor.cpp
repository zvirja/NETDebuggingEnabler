#include "baseset.h"
#include "ProcessHintExtractor.h"
#include <memory>
#include <wx\cmdline.h>

using namespace std;
using namespace ProcessHintExtractorStuff;

namespace Managers
{

	ProcessHintExtractor::ProcessHintExtractor()
	{
		hNtDll = LoadLibrary(L"ntdll.dll");
		if (hNtDll == nullptr)
			return;
		NtQueryInformationProcess = (PNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");
		if (NtQueryInformationProcess == nullptr) {
			FreeLibrary(hNtDll);
			hNtDll = nullptr;
			return;
		}
	}

	ProcessHintExtractor::~ProcessHintExtractor()
	{
		if (hNtDll != nullptr)
			FreeLibrary(hNtDll);
	}


	void ProcessHintExtractor::FillProcessInfoWithHint(DWORD processID, Frames::ProcessInfo& processInfo)
	{
		//Check if import was successful
		if (NtQueryInformationProcess == nullptr)
			return;

		//Apply this filter only to w3wp
		if (!processInfo.GetName().Upper().Contains("W3WP"))
			return;

		//For debug purpose
		/*	if (!processInfo.GetName().Upper().Contains("TRAYGAR"))
		return;*/

		//Open process
		unique_handle process(OpenProcess(PROCESS_ALL_ACCESS, false, processID));
		if (process.get() == INVALID_HANDLE_VALUE || process.get() == NULL)
			return;

		//Get basic info
		PROCESS_BASIC_INFORMATION procBasicInfo;
		ULONG resLen;
		if (NtQueryInformationProcess(process.get(), ProcessBasicInformation, &procBasicInfo, sizeof(procBasicInfo), &resLen) != ERROR_SUCCESS)
			return;

		//Extract PEB from foreign memory
		PEB peb;
		size_t pebResLen;
		if (!ReadProcessMemory(process.get(), procBasicInfo.PebBaseAddress, &peb, sizeof(PEB), &pebResLen))
			return;

		//Extract process parameters from foreign memory
		RTL_USER_PROCESS_PARAMETERS procParams;
		if (!ReadProcessMemory(process.get(), peb.ProcessParameters, &procParams, sizeof(RTL_USER_PROCESS_PARAMETERS), &pebResLen))
			return;

		//Extract command line parameters
		UNICODE_STRING& commandLine = procParams.CommandLine;
		size_t buffSize = commandLine.Length + 2;
		unique_ptr<wchar_t[]> commandLineBuffer = unique_ptr<wchar_t[]>(new wchar_t[buffSize]);
		if (!ReadProcessMemory(process.get(), commandLine.Buffer, commandLineBuffer.get(), buffSize, &pebResLen))
			return;


		wxString rawCommandLine(commandLineBuffer.get());

		//For debug purpose
		//wxString rawCommandLine = L"c:\\windows\\system32\\inetsrv\\w3wp.exe -ap \"sc 660 rev120918 Clean \" -v \"v4.0\" -l \"webengine4.dll\" -a \\\\.\\pipe\\iisipm7153bb97-df74-4162-a277-00d3c9f60094 -h \"C:\\inetpub\\temp\\apppools\\sc660rev120918Clean\\sc660rev120918Clean.config\" -w \"\" -m 0 -t 20";

		wxString hint = ExtractHintFromParams(rawCommandLine);
		if (!hint.IsEmpty())
			processInfo.SetNameHint(hint);
	}

	wxString ProcessHintExtractor::ExtractHintFromParams(const wxString& rawCommandLine)
	{
		auto splittedArgs = wxSplit(rawCommandLine, L' ');
		if (splittedArgs.GetCount() < 2)
			return wxString();
		for (size_t i = 0; i < splittedArgs.GetCount(); i++)
		{
			if (splittedArgs[i].CmpNoCase(L"-ap") == 0)
			{ 
				//Here I collect all string in quotes
				wxString finalHint; 
				bool alreadyInQuote = false;
				
				for (size_t j = i + 1; j < splittedArgs.GetCount(); j++)
				{
					auto currentLine = splittedArgs[j];
					if (currentLine.CmpNoCase(L"\"") == 0)
					{ 
						if (alreadyInQuote)
							break;
						alreadyInQuote = true;
						continue;
					}
					if (currentLine.StartsWith(L"\""))
					{
						if (alreadyInQuote)
							break;
						alreadyInQuote = true;
						currentLine = currentLine.Mid(1, currentLine.length() - 1);
					}
					if (currentLine.EndsWith(L"\""))
					{
						currentLine = currentLine.Mid(0, currentLine.length() - 1);
						finalHint += currentLine + L" ";
						break;
					}
					else
						finalHint += currentLine + L" ";
				}

				return finalHint.Trim();


				//This code doesn't handle quotes
				/*if (splittedArgs.GetCount() > i + 1)
				{
				auto argToReturn = splittedArgs[i + 1];
				if (argToReturn.StartsWith(L"\"") && argToReturn.EndsWith(L"\""))
				return argToReturn.Mid(1, argToReturn.length() - 2);
				return argToReturn;
				}*/
			}
		}
		return wxString();
	}





}