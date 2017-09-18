#include "stdafx.h"

#include"HookTool.h"
#include "..\InjectDll\HookTool.h"


/***********************************************************************/
BOOL hook(LPCSTR dllName, LPCSTR funcName, PROC pfnNew, PBYTE pOrgBytes)
{


	
	MessageBoxW(NULL, CharToWchar(dllName), CharToWchar(funcName), MB_OK);
	FARPROC pfnOrg;
	BYTE pBuf[5] = { 0xE9,0, }; //存放跳转指令
	PBYTE pByte;
	DWORD oldProtect, jumpAddress;
	//主动load我的dll
	MessageBoxW(NULL, L"hook2 ", L"hook2 ", MB_OK);
	HMODULE dllHmodule = LoadLibraryA(dllName);
	MessageBoxW(NULL, L"hook3 ", L"hook3 ", MB_OK);
	pfnOrg = (FARPROC)GetProcAddress(dllHmodule, funcName);
	pByte = (PBYTE)pfnOrg;

	//若已经被钩取，则返回FALSE
	if (pByte[0] == 0xE9)
		MessageBoxW(NULL, L"hook4 ", L"hook4 ", MB_OK);
		//return FALSE;
	
	//为了修改前5个字节，向内存添加“写”属性
	VirtualProtect(pfnOrg, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

	//备份原来函数的前5个字节
	memcpy(pOrgBytes, pfnOrg, 5);

	//计算jmp地址，相对地址
	jumpAddress = (DWORD)pfnNew - (DWORD)pfnOrg - 5;
	memcpy(&pBuf[1], &jumpAddress, 4);

	//修改前5个字节
	memcpy(pfnOrg, &pBuf, 5);
	
	//恢复内存属性
	VirtualProtect(pfnOrg,5,oldProtect,&oldProtect);
	return TRUE;
}






/***************************************************************/
BOOL unHook( LPCSTR dllName,  LPCSTR funcName,  PBYTE pOrgBytes)
{
	FARPROC pFunc;
	DWORD dwOldProtect;
	PBYTE pByte;

	pFunc = (FARPROC)GetProcAddress(GetModuleHandleA(dllName), funcName);
	pByte = (PBYTE)pFunc;


	//若已经脱钩，则返回false
	if (pByte[0] != 0xE9)
		return FALSE;

	//添加写属性，向内存中写入原来的5个字节
	VirtualProtect(pFunc,5,PAGE_EXECUTE_READWRITE,&dwOldProtect);
	
	//脱钩，即写回字节
	memcpy(pFunc,pOrgBytes,5);

	//回复内存属性
	VirtualProtect(pFunc, 5, dwOldProtect, &dwOldProtect);
	return TRUE;


}
wchar_t* CharToWchar(const char* c)
{
	
	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	wchar_t* m_wchar  = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}