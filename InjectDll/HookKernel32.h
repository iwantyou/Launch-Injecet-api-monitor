#pragma once
#include<Windows.h>
#include<Tlhelp32.h>

/************************************************/

 HANDLE WINAPI  myCreateFileW(
	_In_     LPCTSTR               lpFileName,
	_In_     DWORD                 dwDesiredAccess,
	_In_     DWORD                 dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_     DWORD                 dwCreationDisposition,
	_In_     DWORD                 dwFlagsAndAttributes,
	_In_opt_ HANDLE                hTemplateFile
);
 

 HANDLE WINAPI  myCreateFileA(
	 _In_     LPCSTR               lpFileName,
	 _In_     DWORD                 dwDesiredAccess,
	 _In_     DWORD                 dwShareMode,
	 _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	 _In_     DWORD                 dwCreationDisposition,
	 _In_     DWORD                 dwFlagsAndAttributes,
	 _In_opt_ HANDLE                hTemplateFile
 );
 /************************************************/

 HANDLE WINAPI  myOpenThread(
	 _In_ DWORD dwDesiredAccess,
	 _In_ BOOL  bInheritHandle,
	 _In_ DWORD dwThreadId
 );
 /************************************************/


 DWORD WINAPI  myWaitForSingleObject(
	 _In_ HANDLE hHandle,
	 _In_ DWORD  dwMilliseconds
 );
 /************************************************/

 HMODULE WINAPI   myLoadLibraryA(
	 _In_ LPCSTR lpFileName

 );
 HMODULE WINAPI   myLoadLibraryW(
	 _In_ LPCTSTR lpFileName

 );
 /************************************************/


BOOL WINAPI myDeleteFileW(
	 _In_ LPCTSTR lpFileName
 );
/************************************************/

HANDLE WINAPI myCreateMutexA(
	_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
	_In_     BOOL                  bInitialOwner,
	_In_opt_ LPCSTR               lpName
);


HANDLE WINAPI myCreateMutexW(
	_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
	_In_     BOOL                  bInitialOwner,
	_In_opt_ LPCTSTR               lpName
);
/*******************************************************/
BOOL WINAPI myCreateDirectoryW(
	_In_     LPCTSTR               lpPathName,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes
);
/*******************************************************/
BOOL WINAPI mySetFileAttributesW(
	_In_ LPCTSTR lpFileName,
	_In_ DWORD   dwFileAttributes
);
/*******************************************************/
BOOL WINAPI myCopyFileExW(
	_In_     LPCTSTR            lpExistingFileName,
	_In_     LPCTSTR            lpNewFileName,
	_In_opt_ LPPROGRESS_ROUTINE lpProgressRoutine,
	_In_opt_ LPVOID             lpData,
	_In_opt_ LPBOOL             pbCancel,
	_In_     DWORD              dwCopyFlags
);
/******************************************************/
HANDLE WINAPI myGetComputerNameW(
	_Out_   LPCTSTR  lpBuffer,
	_Inout_ LPDWORD lpnSize
);
/******************************************************/
BOOL WINAPI myProcess32Next(
	_In_  HANDLE           hSnapshot,
	_Out_ LPPROCESSENTRY32 lppe
);
/*********************************************/
BOOL WINAPI myTerminateProcess(
	_In_ HANDLE hProcess,
	_In_ UINT   uExitCode
);