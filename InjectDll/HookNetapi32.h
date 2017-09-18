#pragma once
#include<Windows.h>
#include<Lm.h>


/*************************************/
NET_API_STATUS WINAPI myNetShareEnum(
	_In_    LPWSTR  servername,
	_In_    DWORD   level,
	_Out_   LPBYTE  *bufptr,
	_In_    DWORD   prefmaxlen,
	_Out_   LPDWORD entriesread,
	_Out_   LPDWORD totalentries,
	_Inout_ LPDWORD resume_handle
);