#include "stdafx.h"
#include<Windows.h>
#include"HookByConf.h"

using namespace std;
extern map<string, FARPROC> trueApiMap;


//��load dll
HMODULE hMyDll = GetModuleHandleA("MyDll.dll");


//�������е�ǰ dll �µ�api ����ָ��

typedef void(*PMymyfun)(int);



//�����ж���ĺ���ָ�븳ֵ(ԭ��api �ĵ�ַ)


//����ĺ���


 void mymyfun(int i)
	{
		
		//OutputDebugString(L"hooking fun");
		PMymyfun trueMYfun = (PMymyfun)trueApiMap.find("myfun")->second;
		return trueMYfun(i);
	}
