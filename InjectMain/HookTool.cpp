#include "stdafx.h"

#include"HookTool.h"
#include "..\InjectDll\HookTool.h"


/***********************************************************************/
BOOL hook(LPCSTR dllName, LPCSTR funcName, PROC pfnNew, PBYTE pOrgBytes)
{


	
	MessageBoxW(NULL, CharToWchar(dllName), CharToWchar(funcName), MB_OK);
	FARPROC pfnOrg;
	BYTE pBuf[5] = { 0xE9,0, }; //�����תָ��
	PBYTE pByte;
	DWORD oldProtect, jumpAddress;
	//����load�ҵ�dll
	MessageBoxW(NULL, L"hook2 ", L"hook2 ", MB_OK);
	HMODULE dllHmodule = LoadLibraryA(dllName);
	MessageBoxW(NULL, L"hook3 ", L"hook3 ", MB_OK);
	pfnOrg = (FARPROC)GetProcAddress(dllHmodule, funcName);
	pByte = (PBYTE)pfnOrg;

	//���Ѿ�����ȡ���򷵻�FALSE
	if (pByte[0] == 0xE9)
		MessageBoxW(NULL, L"hook4 ", L"hook4 ", MB_OK);
		//return FALSE;
	
	//Ϊ���޸�ǰ5���ֽڣ����ڴ���ӡ�д������
	VirtualProtect(pfnOrg, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

	//����ԭ��������ǰ5���ֽ�
	memcpy(pOrgBytes, pfnOrg, 5);

	//����jmp��ַ����Ե�ַ
	jumpAddress = (DWORD)pfnNew - (DWORD)pfnOrg - 5;
	memcpy(&pBuf[1], &jumpAddress, 4);

	//�޸�ǰ5���ֽ�
	memcpy(pfnOrg, &pBuf, 5);
	
	//�ָ��ڴ�����
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


	//���Ѿ��ѹ����򷵻�false
	if (pByte[0] != 0xE9)
		return FALSE;

	//���д���ԣ����ڴ���д��ԭ����5���ֽ�
	VirtualProtect(pFunc,5,PAGE_EXECUTE_READWRITE,&dwOldProtect);
	
	//�ѹ�����д���ֽ�
	memcpy(pFunc,pOrgBytes,5);

	//�ظ��ڴ�����
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