#include "stdafx.h"
#include"Client.h"
#include "HookAdvapi32.h"
#include"HookByConf.h"
#include"StrTool.h"

extern map<string, FARPROC> trueApiMap;

extern SOCKET ClientSocket;



extern CRITICAL_SECTION LOCK;
typedef SC_HANDLE(WINAPI* POpenServiceW)(SC_HANDLE, LPCTSTR, DWORD);
/***********************************************/


SC_HANDLE WINAPI myOpenServiceW(SC_HANDLE hSCManager, LPCTSTR lpServiceName, DWORD dwDesiredAccess)
{	
	EnterCriticalSection(&LOCK);

	char SendBuffer[BUFSIZE] = {};
	char *c1 = "OpenServiceW(lpServiceName:";
	char *c2 = WchartoChar(lpServiceName);

	strcat_s(SendBuffer, BUFSIZE, c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");

	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer) + 1, 0);

	LeaveCriticalSection(&LOCK);


	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send OpenServiceW ERR", L"send OpenServiceW ERR", MB_OK);
	}
	POpenServiceW trueOpenServiceW=(POpenServiceW)trueApiMap.find("OpenServiceW")->second;

	return trueOpenServiceW(hSCManager, lpServiceName, dwDesiredAccess);
}
