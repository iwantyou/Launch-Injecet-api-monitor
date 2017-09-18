#include "stdafx.h"
#include"HookByConf.h"
#include"StrTool.h"
#include<iterator>
#include<map>
#include"ReadConf.h"
#include"mhook-lib\mhook.h"
#include"HookKernel32.h"
using namespace std;
extern map<string, vector<string>> dllMap;
map<string, FARPROC> trueApiMap;
VOID hookByConf(HMODULE hInstance)
{
	
	for (map<string, vector<string>>::iterator it1 = dllMap.begin();it1!= dllMap.end();it1++ )
	{
		//string tmp = *it;
		
		string dllName = it1->first;
		vector<string> apiVetor = it1->second;
		for (vector<string > ::iterator it2 = apiVetor.begin();it2!=apiVetor.end();it2++)
		{ 
			
			string apiName = *it2;
			string myApiName = "my" + apiName;
			
			//MessageBoxW(NULL, charToWchar(apiName.c_str()), charToWchar(myApiName.c_str()), MB_OK);

			HMODULE hDll = LoadLibraryA(dllName.c_str());
			if (hDll == NULL)
			{
				//MessageBoxW(NULL, L"11", L"11", MB_OK);
				return;
			}
			
			FARPROC trueApi = GetProcAddress(hDll,apiName.c_str());
			if (true == NULL)
			{
				return;
			}
			


			FARPROC myApi = GetProcAddress(hInstance,myApiName.c_str());
			
			if (myApi==NULL)
			{
				//MessageBoxW(NULL, L"33", L"33", MB_OK);
				return;
			}
			//MessageBoxW(NULL, L"44", L"44", MB_OK);
			//trueApiMap.insert(pair<string,FARPROC>(apiName, trueApi));
			//Mhook_SetHook((PVOID*)&trueApi,myApi);
			Mhook_SetHook((PVOID*)&trueApi,myApi);
			trueApiMap.insert(pair<string, FARPROC>(apiName, trueApi));
			
		}
		
	}
	
}



VOID unhoookByConf(HMODULE hInstance)
{
	for (map<string, vector<string>>::iterator it1 = dllMap.begin(); it1 != dllMap.end(); it1++)
	{
		//string tmp = *it;
		string dllName = it1->first;
		vector<string> apiVetor = it1->second;
		for (vector<string > ::iterator it2 = apiVetor.begin(); it2 != apiVetor.end(); it2++)
		{
			string apiName = *it2;
			HMODULE hDll = LoadLibraryA(dllName.c_str());

			FARPROC trueApi = GetProcAddress(hDll, apiName.c_str());
			Mhook_Unhook((PVOID*)&trueApi);

		}

	}
}

