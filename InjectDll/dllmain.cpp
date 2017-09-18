// dllmain.cpp : Defines the entry point for the DLL application.
  

#include "stdafx.h"
#include"Client.h"
#include"ReadConf.h"
#include <vector>
#include<map>
#include<string>
#include"HookByConf.h"



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	
	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		
		MessageBoxW(NULL,L"inject dll succeed", L"inject dll succeed", MB_OK);
		connectServer();
		/*读取配置文件，获取dllMap*/
		
		readConf(hModule, "D:\\DllConfig.json");//my pc
		//readConf(hModule, "C:\\Users\\admin\\Desktop\\DllConfig.json");//VIT pc
		/*根据配置文件hook*/
		hookByConf(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		unhoookByConf(hModule);
		break;
	}
	return TRUE;
}

