// InjectMain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<Windows.h>


#include<iostream>




int main()
{
	//MessageBoxW(NULL,L"hahah",L"hahah", MB_OK);
	//ö�����н���
    /*HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);
	if (Process32First(snapshot, &process))
	{
		do
		{
			printf("%S %d\n",process.szExeFile,process.th32ProcessID);
		} while (Process32Next(snapshot, &process));

	}
	CloseHandle(snapshot);*/

	//��ȡ������̵ľ��
	/*int pid ;
	printf("input hook process pid:");
	scanf("%d",&pid);*/

	


	LPVOID pRemoteBuf = NULL;
	HANDLE hThread;
	HANDLE hProcess;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	si = { sizeof(si) };
	TCHAR szCommandLine[] = TEXT("WeChat.exe");
	
	/* *************************      ����Զ���߳�******************************/
	//HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	BOOL status = CreateProcess(TEXT("E:\\WeChat\\WeChat.exe"), szCommandLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	if (!status)
	{
		MessageBox(0, L"CreateProcess error", 0, 0);
	}
	DWORD pid = pi.dwProcessId;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	//LPCTSTR dllNamePath = L"C:\\Users\\admin\\Desktop\\InjectDll.dll"; //windows7ʱ
	//LPCTSTR dllNamePath = L"D:\\NewCode\\InjectDemo\\Debug\\InjectDll.dll"; //windows10ʱ
	LPCTSTR dllNamePath = L"D:\\InjectDemo\\Debug\\InjectDll.dll";//my pc
	DWORD dwBufSize = (DWORD)(wcslen(dllNamePath) + 1) * sizeof(TCHAR);
	//��hProcess���̵��ڴ�������һ���ڴ�
	pRemoteBuf = ::VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);
	//��������ڴ���д����
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)dllNamePath, dwBufSize, NULL);
	HMODULE module = ::GetModuleHandle(TEXT("Kernel32.dll"));
	if(module == NULL)
	{
		int err = ::GetLastError();
	}
	LPTHREAD_START_ROUTINE addr = (LPTHREAD_START_ROUTINE)GetProcAddress(module, "LoadLibraryW");

	//����Զ���߳�
	hThread = ::CreateRemoteThread(hProcess, NULL, 0, addr, pRemoteBuf, 0, NULL);
	if (hThread == NULL)
	{
		int err = ::GetLastError();
	}
	::WaitForSingleObject(hThread, INFINITE);
	::CloseHandle(hThread);

	
	ResumeThread(pi.hThread);

	

	return 0;
}

