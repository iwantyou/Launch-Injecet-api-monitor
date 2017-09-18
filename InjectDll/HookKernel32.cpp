#include "stdafx.h"

#include"Client.h"

#include<Windows.h>
#include"HookByConf.h"
#include <atlstr.h>
#include "HookKernel32.h"
#include<string>
#include"StrTool.h"
//#include"easyloggingpp_v9.95.0\easylogging++.h"
using namespace std;
extern map<string, FARPROC> trueApiMap;

extern SOCKET ClientSocket;



extern CRITICAL_SECTION LOCK;


//CRITICAL_SECTION g_cs;
//HMODULE hKernel32Dll = GetModuleHandleA("Kernel32.dll");

/******************************************一系列函数指针*********************************/
typedef HANDLE(WINAPI* PCreateFileW)(
	_In_     LPCTSTR               ,
	_In_     DWORD                 ,
	_In_     DWORD                 ,
	_In_opt_ LPSECURITY_ATTRIBUTES ,
	_In_     DWORD                 ,
	_In_     DWORD                 ,
	_In_opt_ HANDLE                
	);

typedef HANDLE(WINAPI* PCreateFileA)(
	_In_     LPCSTR,
	_In_     DWORD                 ,
	_In_     DWORD                 ,
	_In_opt_ LPSECURITY_ATTRIBUTES ,
	_In_     DWORD                 ,
	_In_     DWORD                 ,
	_In_opt_ HANDLE                
	);



typedef HANDLE(WINAPI* POpenThread)(_In_ DWORD ,_In_ BOOL  ,_In_ DWORD );

typedef DWORD(WINAPI* PWaitForSingleObject) (HANDLE, DWORD);

typedef HMODULE(WINAPI* PLoadLibraryA) (LPCSTR);

typedef HMODULE(WINAPI* PLoadLibraryW) (LPCTSTR);

typedef BOOL(WINAPI* PDeleteFile)(LPCTSTR);

typedef HANDLE(WINAPI* PCreateMutexA) (LPSECURITY_ATTRIBUTES, BOOL, LPCSTR);

typedef HANDLE(WINAPI* PCreateMutexW) (LPSECURITY_ATTRIBUTES, BOOL, LPCTSTR);

typedef BOOL(WINAPI* PCreateDirectoryW)(LPCTSTR, LPSECURITY_ATTRIBUTES);

typedef BOOL(WINAPI* PSetFileAttributesW)(LPCTSTR,DWORD);

typedef BOOL(WINAPI* PCopyFileExW)(LPCTSTR, LPCTSTR, LPPROGRESS_ROUTINE, LPVOID, LPBOOL, DWORD);

typedef HANDLE(WINAPI* PGetComputerNameW)(LPCTSTR, LPDWORD);

typedef BOOL(WINAPI* PProcess32Next)(HANDLE, LPPROCESSENTRY32);

typedef BOOL(WINAPI* PTerminateProcess)(HANDLE, UINT);
 /**********************************************替代的api*****************************************/

HANDLE  WINAPI  myCreateFileA(
	_In_     LPCSTR               lpFileName,
	_In_     DWORD                 dwDesiredAccess,
	_In_     DWORD                 dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_     DWORD                 dwCreationDisposition,
	_In_     DWORD                 dwFlagsAndAttributes,
	_In_opt_ HANDLE                hTemplateFile
)
{
	
	//LOG(INFO) << "HOOK CreateFileA[FileName:"<< lpFileName<<"]"<<endl;
	PCreateFileA trueCreateFileW = (PCreateFileA)trueApiMap.find("CreateFileA")->second;
	return trueCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition,
		dwFlagsAndAttributes, hTemplateFile);

}

HANDLE  WINAPI  myCreateFileW(_In_ LPCTSTR lpFileName, _In_  DWORD dwDesiredAccess, _In_ DWORD dwShareMode, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, _In_  DWORD dwCreationDisposition, _In_  DWORD dwFlagsAndAttributes, _In_opt_ HANDLE hTemplateFile)
{
	
	EnterCriticalSection(&LOCK);

	char SendBuffer[BUFSIZE] = {};
	char *c1 = "CreateFileW(FileName:";
	char *c2 = WchartoChar(lpFileName);
	
	strcat_s(SendBuffer, BUFSIZE,c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, "\n\0");

	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer)+1, 0);
	
	LeaveCriticalSection(&LOCK);

	
	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send CreateFileW ERR", L"send OpenThread ERR", MB_OK);
	}
	PCreateFileW trueCreateFileW = (PCreateFileW)trueApiMap.find("CreateFileW")->second;
	return trueCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition,
		dwFlagsAndAttributes, hTemplateFile);
	
}
/***************************************************************/
HANDLE WINAPI myOpenThread(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId)
{
	

	EnterCriticalSection(&LOCK);
	char SendBuffer[BUFSIZE] = {};
	
	char* c1 = "OpenThread(ThreadId:";
	char c2[20];
	
	_ultoa_s(dwThreadId, c2, 20, 10);
	
	strcat_s(SendBuffer, BUFSIZE, c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");


	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer)+1, 0);
	LeaveCriticalSection(&LOCK);
	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send OpenThread ERR", L"send OpenThread ERR", MB_OK);
	}
	POpenThread trueOpenThread = (POpenThread)trueApiMap.find("OpenThread")->second;
	return trueOpenThread(dwDesiredAccess, bInheritHandle, dwThreadId);
	
}
/***************************************************************/
DWORD WINAPI myWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds)
{

	
	EnterCriticalSection(&LOCK);
	
	char SendBuffer[BUFSIZE] = {};
	char* c1 = "WaitForSingleObject(hHandle:";
	char c2[32];
	DWORD h = (DWORD)hHandle;
	_ultoa_s(h, c2, 20, 16);
	

	strcat_s(SendBuffer, BUFSIZE, c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");


	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer) + 1, 0);
	LeaveCriticalSection(&LOCK);
	
	LeaveCriticalSection(&LOCK);
	/*if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send WaitForSingleObject ERR", L"send OpenThread ERR", MB_OK);
	}*/
	PWaitForSingleObject trueWaitForSingleObject = (PWaitForSingleObject)trueApiMap.find("WaitForSingleObject")->second;
	return trueWaitForSingleObject(hHandle, dwMilliseconds);
}
/***************************************************************/
HMODULE WINAPI myLoadLibraryA(LPCSTR  lpFileName)
{
	
	//LOG(INFO) << "HOOK LoadLibraryA[FileName:" << lpFileName<<"]"<<endl;

	PLoadLibraryA trueLoadLibraryA = (PLoadLibraryA)trueApiMap.find("LoadLibraryA")->second;
	return trueLoadLibraryA(lpFileName);
}

HMODULE WINAPI myLoadLibraryW(LPCTSTR lpFileName)
{
	
	
	EnterCriticalSection(&LOCK);
	
	char SendBuffer[BUFSIZE] = {};
	
	char* c2 = WchartoChar(lpFileName);
	char * c1 = "LoadLibraryW(FileName:";
	strcat_s(SendBuffer, BUFSIZE, c1);
	
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");
	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer)+1, 0);
	LeaveCriticalSection(&LOCK);
	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send LoadLibraryW ERR", L"send OpenThread ERR", MB_OK);
	}

	PLoadLibraryW trueLoadLibraryW = (PLoadLibraryW)trueApiMap.find("LoadLibraryW")->second;
	return trueLoadLibraryW(lpFileName);
}
/***************************************************************/
BOOL WINAPI myDeleteFileW(LPCTSTR lpFileName)
{
	
	
	PDeleteFile trueDeleteFile = (PDeleteFile)trueApiMap.find("DeleteFileW")->second;
	return trueDeleteFile(lpFileName);
}
/***************************************************************/
HANDLE WINAPI myCreateMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName)
{
	
	//LOG(INFO) << "HOOK CreateMutexA[]" << endl;


	PCreateMutexA trueCreateMutexA = (PCreateMutexA)trueApiMap.find("CreateMutexA")->second;
	return trueCreateMutexA(lpMutexAttributes, bInitialOwner, lpName);
}
/***************************************************************/
HANDLE WINAPI myCreateMutexW(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName)
{

	
	
	EnterCriticalSection(&LOCK);
	char SendBuffer[BUFSIZE] = {};
	char* c1 = "CreateMutexW(lpName:";
	char* c2 = WchartoChar(lpName);
	
	strcat_s(SendBuffer, BUFSIZE,c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");
	LeaveCriticalSection(&LOCK);
	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer)+1, 0);
	
	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send CreateMutexW ERR", L"send OpenThread ERR", MB_OK);
	}

	PCreateMutexW trueCreateMutexW = (PCreateMutexW)trueApiMap.find("CreateMutexW")->second;
	return trueCreateMutexW(lpMutexAttributes, bInitialOwner, lpName);
	//
}
/*************************************************************/
BOOL WINAPI myCreateDirectoryW(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	
	
	
	EnterCriticalSection(&LOCK);
	
	char SendBuffer[BUFSIZE] = {};
	
	char* c1 = "CreateDirectoryW(lpPathName:";
	char* c2 = WchartoChar(lpPathName);

	strcat_s(SendBuffer, BUFSIZE,c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");
	LeaveCriticalSection(&LOCK);
	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer)+1, 0);

	
	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send CreateDirectoryW ERR", L"send OpenThread ERR", MB_OK);
	}

	PCreateDirectoryW trueCreateDirectoryW = (PCreateDirectoryW)trueApiMap.find("CreateDirectoryW")->second;
	return trueCreateDirectoryW(lpPathName, lpSecurityAttributes);
}
/***************************************************************/
BOOL WINAPI mySetFileAttributesW(LPCTSTR lpFileName, DWORD dwFileAttributes)
{
	EnterCriticalSection(&LOCK);
	
	char SendBuffer[BUFSIZE] = {};
	
	char* c1 = "SetFileAttributesW(lpFileName:";
	char* c2 = WchartoChar(lpFileName);

	strcat_s(SendBuffer, BUFSIZE, c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");
	LeaveCriticalSection(&LOCK);
	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer) + 1, 0);


	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send FileAttributesW ERR", L"send FileAttributesW ERR", MB_OK);
	}
	PSetFileAttributesW trueSetFileAttributesW = (PSetFileAttributesW)trueApiMap.find("SetFileAttributesW")->second;
	return trueSetFileAttributesW(lpFileName, dwFileAttributes);
}
/**************************************************************/
BOOL WINAPI myCopyFileExW(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, LPPROGRESS_ROUTINE lpProgressRoutine, LPVOID lpData, LPBOOL pbCancel, DWORD dwCopyFlags)
{
	
	EnterCriticalSection(&LOCK);

	char SendBuffer[BUFSIZE] = {};

	char* c1 = "CopyFileExW(lpPathName:";
	char* c2 = WchartoChar(lpExistingFileName);
	char* c3 = ",lpNewFileName:";
	char* c4 = WchartoChar(lpNewFileName);
	
	strcat_s(SendBuffer, BUFSIZE, c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, c3);
	strcat_s(SendBuffer, BUFSIZE, c4);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");
	LeaveCriticalSection(&LOCK); 
	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer) + 1, 0);


	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send CopyFileExW ERR", L"send CopyFileExW ERR", MB_OK);
	}
	PCopyFileExW trueCopyFileExW = (PCopyFileExW)trueApiMap.find("CopyFileExW")->second;
	return trueCopyFileExW(lpExistingFileName, lpNewFileName, lpProgressRoutine, lpData, pbCancel, dwCopyFlags);

}
/************************************************************/
HANDLE WINAPI myGetComputerNameW(LPCTSTR lpBuffer, LPDWORD lpnSize)
{
	EnterCriticalSection(&LOCK);

	char SendBuffer[BUFSIZE] = {};

	char* c1 = "GetComputerNameW:lpBuffer";
	char* c2 = WchartoChar(lpBuffer);

	strcat_s(SendBuffer, BUFSIZE, c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");
	LeaveCriticalSection(&LOCK);
	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer) + 1, 0);


	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send GetComputerNameW ERR", L"send GetComputerNameW ERR", MB_OK);
	}
	
	PGetComputerNameW trueGetComputerNameW = (PGetComputerNameW)trueApiMap.find("GetComputerNameW")->second;

	return trueGetComputerNameW(lpBuffer,lpnSize);
}
/**************************************************************/
BOOL WINAPI myProcess32Next(HANDLE hSnapshot, LPPROCESSENTRY32 lppe)
{
	
	EnterCriticalSection(&LOCK);

	char SendBuffer[BUFSIZE] = {};

	char* c1 = "Process32Next(hSnapshot:";
	
	char c2[32];
	DWORD h = (DWORD)hSnapshot;
	_ultoa_s(h, c2, 32, 16);
	strcat_s(SendBuffer, BUFSIZE, c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");
	LeaveCriticalSection(&LOCK);
	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer) + 1, 0);


	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send Process32Next ERR", L"send Process32Next ERR", MB_OK);
	}
	PProcess32Next trueProcess32Next = (PProcess32Next)trueApiMap.find("Process32Next")->second;

	return trueProcess32Next(hSnapshot,lppe);
}
/****************************************************/
BOOL WINAPI myTerminateProcess(HANDLE hProcess, UINT uExitCode)
{
	
	
	EnterCriticalSection(&LOCK);
	
	char SendBuffer[BUFSIZE] = {};
	char* c1 = "TerminateProcess(hProcess:";
	char c2[32];
	DWORD h = (DWORD)hProcess;
	_ultoa_s(h, c2, 20, 16);
	strcat_s(SendBuffer, BUFSIZE,c1);
	strcat_s(SendBuffer, BUFSIZE, c2);
	strcat_s(SendBuffer, BUFSIZE, ")\n\0");
	LeaveCriticalSection(&LOCK);
	int ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer)+1, 0);
	
	if (ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"send TerminateProcess ERR", L"send OpenThread ERR", MB_OK);
	}

	PTerminateProcess trueTerminateProcess = (PTerminateProcess)trueApiMap.find("TerminateProcess")->second;
	return trueTerminateProcess(hProcess,uExitCode);
}

/***************************************************************/