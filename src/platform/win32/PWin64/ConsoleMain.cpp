#include <iostream>
#include <Windows.h>
#include "Browser.h"

#define STR_MATCH 0

enum OpCodes
{
	UNKNOWN,
	BROWSER,
};

struct CommandLine
{
	wchar_t* m_pszTempFilePath;
	wchar_t* m_pszUrl;		
	wchar_t* m_pszCallback;

	enum OpCodes m_opCode;
};

bool ProcessCmdl(struct CommandLine* pCmdl, int argc, wchar_t** argv);


int wmain(int argc, wchar_t** argv)
{
	struct CommandLine cmdl = { 0 };

	if (!ProcessCmdl(&cmdl, argc, argv))
		return -1;

	if (cmdl.m_opCode == BROWSER)
	{
		if (cmdl.m_pszCallback == nullptr ||
			cmdl.m_pszTempFilePath == nullptr ||
			cmdl.m_pszUrl == nullptr)		
			return -2;		

		HINSTANCE appHandle = GetModuleHandle(NULL);
		HINSTANCE hInstance = static_cast<HINSTANCE>(appHandle);

		AdBrowser* frame = new AdBrowser(hInstance, L"Test");
		frame->ShowWindow(NULL, TRUE, cmdl.m_pszUrl);

		delete frame;
	}
	
	delete[] cmdl.m_pszTempFilePath;
	delete[] cmdl.m_pszUrl;
	delete[] cmdl.m_pszCallback;
}

bool ProcessCmdl(struct CommandLine* pCmdl, int argc, wchar_t** argv)
{
	while (++argv && --argc)
	{
		wchar_t* ptr = *argv;

		if (*ptr == '-')
		{
			ptr++;

			if (lstrcmpi(ptr, L"temp") == STR_MATCH)
			{
				if (argc > 1)
				{
					++argv; --argc;
					ptr = *argv;

					size_t len = lstrlen(ptr);

					pCmdl->m_pszTempFilePath = new wchar_t[len + 1];
					memset(pCmdl->m_pszTempFilePath, 0, len);

					lstrcpy(pCmdl->m_pszTempFilePath, ptr);
				}
				else
					return false;

				continue;
			}
			else if (lstrcmpi(ptr, L"url") == STR_MATCH)
			{
				if (argc > 1)
				{
					++argv; --argc;
					ptr = *argv;

					size_t len = lstrlen(ptr);

					pCmdl->m_pszUrl = new wchar_t[len + 1];
					memset(pCmdl->m_pszUrl, 0, len);

					lstrcpy(pCmdl->m_pszUrl, ptr);
				}
				else
					return false;

				continue;
			}
			else if (lstrcmpi(ptr, L"mode") == STR_MATCH)
			{
				if (argc > 1)
				{
					++argv; --argc;
					ptr = *argv;

					if (lstrcmpi(ptr, L"browser") == STR_MATCH)
						pCmdl->m_opCode = BROWSER;					
				}
				else
					return false;

				continue;
			}
			else if (lstrcmpi(ptr, L"callback") == STR_MATCH)
			{
				if (argc > 1)
				{
					++argv; --argc;
					ptr = *argv;

					size_t len = lstrlen(ptr);

					pCmdl->m_pszCallback = new wchar_t[len + 1];
					memset(pCmdl->m_pszCallback, 0, len);

					lstrcpy(pCmdl->m_pszCallback, ptr);
				}
				else
					return false;

				continue;
			}
		}
	}

	return true;
}
