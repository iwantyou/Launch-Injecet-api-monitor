#include "stdafx.h"
#include "ReadConf.h"
#include"jsoncpp\include\json.h"
#include<fstream>
#include<iostream>
#include"HookByConf.h"
#include<Windows.h>
#include<map>

using namespace std;
map<string, vector<string>> dllMap;


#ifndef DEBUG
#pragma comment(lib,"jsoncpp\\lib\\lib_json_d.lib")
#else
#pragma comment(lib,"jsoncpp\\lib\\lib_json.lib")
#endif  

Json::Reader reader;
Json::Value root;



VOID readConf(HINSTANCE hInstance,string confPath)
{
	ifstream ifs;
	ifs.open(confPath);

	if (!ifs.is_open())
	{
		//MessageBox(NULL, L"open fail", L"open fail", MB_OK);
		return;
	}
	else
	{
		//MessageBox(NULL, L"open succeed", L"open succeed", MB_OK);
	}
	//判断json格式是否正确

	if (!reader.parse(ifs, root, false))
	{
		//MessageBox(NULL, L"parse fail", L"parse fail", MB_OK);
		return;
	}
	else
	{
		//MessageBox(NULL, L"parse succeed", L"parse succeed", MB_OK);
	}
	string dllName;
	string apiName;
	BOOL isHook;
	int size = root.size();
	for (int i = 0; i<size; i++)
	{
		dllName = root[i]["dllName"].asString();
		Json::Value apiList = root[i]["methodList"];
		int apiSize = apiList.size();
		vector<string> apiVector;
		BOOL f = FALSE;
		for (int j = 0; j<apiSize; j++)
		{
			
			isHook = apiList[j]["hook"].asBool();
			if (isHook)
			{
				f = TRUE;
				apiName = apiList[j]["method"].asString();
				apiVector.push_back(apiName);

			}
		}
		if (f)
		{
			//MessageBoxW(NULL, L"dllMap is ok", L"dllMap is ok", MB_OK);
			dllMap.insert(pair<string, vector<string>>(dllName, apiVector));
		}
		
	}
	

}