// Server.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <winsock2.h>
#include<fstream>
#include<iostream>
#define BUFSIZE 9182


#pragma comment(lib, "ws2_32.lib")

#define ELPP_THREAD_SAFE


#include"easylogging++.h"

INITIALIZE_EASYLOGGINGPP

el::Configurations conf("D:\\my_log.conf");

std::ofstream ofile;

int main()
{
	el::Loggers::reconfigureAllLoggers(conf);

	WSADATA            wsad;            //WSADATA变量
	SOCKET            Server;        //服务器套接字
	SOCKET            Client;        //客户端套接字
	SOCKADDR_IN        addrServ;        //服务器地址
	char            buf[BUFSIZE];        //接收数据缓冲区
	int                retVal;        //返回值

	ofile.open("D:\\API_logs.log");
									  //初始化套接字动态库
	if (WSAStartup(MAKEWORD(2, 2), &wsad) != 0)
	{
		printf("初始化套接字动态库失败!\n");
		return 1;
	}

	//创建套接字
	Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == Server)
	{
		printf("创建套接字失败!\n");
		WSACleanup();//释放套接字资源;
		return  -1;
	}

	//服务器套接字地址 
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(4000);
	addrServ.sin_addr.s_addr = INADDR_ANY;


	//绑定套接字
	retVal = bind(Server, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		printf("绑定套接字失败!\n");
		closesocket(Server);    //关闭套接字
		WSACleanup();            //释放套接字资源;
		return -1;
	}

	//开始监听 
	retVal = listen(Server, 1);
	if (SOCKET_ERROR == retVal)
	{
		printf("监听失败!\n");
		closesocket(Server);    //关闭套接字
		WSACleanup();            //释放套接字资源;
		return -1;
	}

	//接受客户端请求
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	printf("等待接受客户端请求!\n");
	Client = accept(Server, (sockaddr FAR*)&addrClient, &addrClientlen);
	if (INVALID_SOCKET == Client)
	{
		printf("接受客户端请求失败!\n");
		closesocket(Server);    //关闭套接字
		WSACleanup();            //释放套接字资源;
		return -1;
	}
	else
	{
		printf("接受客户端成功!\n");
	}

	//接收客户端数据

	while (true)
	{
		printf("从socket缓存读\n");
		ZeroMemory(buf, BUFSIZE);
		retVal = recv(Client, buf, BUFSIZE, 0);
		if (SOCKET_ERROR == retVal)
		{
			printf("接收客户端请求失败!\n");
			closesocket(Server);    //关闭套接字
			closesocket(Client);    //关闭套接字        
			WSACleanup();            //释放套接字资源;
			return -1;
		}
		//输出到log
		LOG(INFO) << buf << std::endl;
		//ofile << buf;
		//std::cout << buf << std::endl;   //输出来自Client的字符串    
	}


	//退出
	closesocket(Server);    //关闭套接字
	closesocket(Client);    //关闭套接字
	WSACleanup();            //释放套接字资源;
	return 0;
}

