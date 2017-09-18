#pragma once
#include<Windows.h>
#include"HookByConf.h"

extern map<string, FARPROC> trueApiMap;


SC_HANDLE WINAPI myOpenServiceW(
	_In_ SC_HANDLE hSCManager,
	_In_ LPCTSTR   lpServiceName,
	_In_ DWORD     dwDesiredAccess
);