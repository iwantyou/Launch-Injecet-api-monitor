#include "stdafx.h"
#include"Client.h"

#include<Windows.h>
#include "HookUser32.h"
#include"HookByConf.h"
#include"StrTool.h"

extern map<string, FARPROC> trueApiMap;

extern SOCKET ClientSocket;



extern CRITICAL_SECTION LOCK;

typedef SHORT(WINAPI* pGetAsyncKeyState)(int);

typedef BOOL(WINAPI* pDestroyWindow)(HWND);
/*******************************************/

SHORT WINAPI myGetAsyncKeyState(int vKey)
{

	
	EnterCriticalSection(&LOCK);

	char SendBuffer[BUFSIZE] = {};
	char *c1 = "GetAsyncKeyState(vKey:";
	char c2[32];
	_itoa_s(vKey,c2,16);
	
	
	strcat_s(SendBuffer, BUFSIZE, c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");

	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer) + 1, 0);

	LeaveCriticalSection(&LOCK);


	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send GetAsyncKeyState ERR", L"send GetAsyncKeyState ERR", MB_OK);
	}


	pGetAsyncKeyState trueGetAsyncKeyState = (pGetAsyncKeyState)trueApiMap.find("GetAsyncKeyState")->second;
	return trueGetAsyncKeyState(vKey);
}
/*****************************************/
BOOL WINAPI myDestroyWindow(HWND hWnd)
{

	EnterCriticalSection(&LOCK);

	char SendBuffer[BUFSIZE] = {};
	char *c1 = "DestroyWindow(hWnd:";
	char c2[32];
	DWORD h = (DWORD)hWnd;
	_ultoa_s(h, c2, 20, 16);

	strcat_s(SendBuffer, BUFSIZE, c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");

	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer) + 1, 0);

	LeaveCriticalSection(&LOCK);


	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send DestroyWindow ERR", L"send DestroyWindow ERR", MB_OK);
	}
	
	pDestroyWindow trueDestroyWindow = (pDestroyWindow)trueApiMap.find("DestroyWindow")->second;
	return trueDestroyWindow(hWnd);
}
