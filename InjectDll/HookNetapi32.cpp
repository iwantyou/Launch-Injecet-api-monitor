#include "stdafx.h"
#include"Client.h"
#include "HookNetapi32.h"
#include"HookByConf.h"
#include"StrTool.h"
extern map<string, FARPROC> trueApiMap;
extern SOCKET ClientSocket;



extern CRITICAL_SECTION LOCK;

typedef NET_API_STATUS(WINAPI*pNetShareEnum)(LPWSTR, DWORD, LPBYTE*, DWORD, LPDWORD, LPDWORD, LPDWORD);


/*********************************/
NET_API_STATUS WINAPI myNetShareEnum(LPWSTR servername, DWORD level, LPBYTE * bufptr, DWORD prefmaxlen, LPDWORD entriesread, LPDWORD totalentries, LPDWORD resume_handle)
{
	EnterCriticalSection(&LOCK);

	char SendBuffer[BUFSIZE] = {};
	char *c1 = "NetShareEnum(servername:";
	char *c2 = WchartoChar(servername);

	strcat_s(SendBuffer, BUFSIZE, c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");

	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer) + 1, 0);

	LeaveCriticalSection(&LOCK);


	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send NetShareEnum ERR", L"send NetShareEnum ERR", MB_OK);
	}

	pNetShareEnum trueNetShareEnum = (pNetShareEnum)trueApiMap.find("NetShareEnum")->second;
	return trueNetShareEnum(servername, level, bufptr, prefmaxlen, entriesread, totalentries, resume_handle);
}
