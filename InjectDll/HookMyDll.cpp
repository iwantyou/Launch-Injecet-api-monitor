#include "stdafx.h"
#include<Windows.h>
#include"HookByConf.h"

using namespace std;
extern map<string, FARPROC> trueApiMap;


//先load dll
HMODULE hMyDll = GetModuleHandleA("MyDll.dll");


//定义所有当前 dll 下的api 函数指针

typedef void(*PMymyfun)(int);



//给所有定义的函数指针赋值(原来api 的地址)


//替代的函数


 void mymyfun(int i)
	{
		
		//OutputDebugString(L"hooking fun");
		PMymyfun trueMYfun = (PMymyfun)trueApiMap.find("myfun")->second;
		return trueMYfun(i);
	}
