#include "stdafx.h"
#include "Client.h"
#include<Windows.h>


//#define ELPP_THREAD_SAFE

//#include"easyloggingpp_v9.95.0\easylogging++.h"


//INITIALIZE_EASYLOGGINGPP

//el::Configurations conf("D:\\my_log.conf");

#define PORT 4000
#define IP_ADDRESS "127.0.0.1"

WSADATA Ws;
SOCKET ClientSocket;
struct sockaddr_in ServerAddr; //服务器地址
int Ret = 0;
int AddrLen = 0;
HANDLE hThread = NULL;


CRITICAL_SECTION LOCK;

void connectServer()
{

	//el::Loggers::reconfigureAllLoggers(conf);


	
	InitializeCriticalSection(&LOCK);
	//Init Windows Socket 初始化套接字动态库
	
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		MessageBoxW(NULL, L"Init Windows Socket FAILD", L"Init Windows Socket FAILD", MB_OK);
		return ;
	}
	//Create Socket
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET)
	{
		MessageBoxW(NULL, L"Init Client Socket FAILD", L"Init Client Socket FAILD", MB_OK);
		return ;
	}

	//设置服务器地址
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = htons(PORT);
	memset(ServerAddr.sin_zero, 0x00, 8);
	//连接服务器
	Ret = connect(ClientSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
	if (Ret == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L" Client connect SERVER FAILD", L" Client connect SERVER FAILD", MB_OK);
		closesocket(ClientSocket); //关闭套接字
		WSACleanup();				//释放套接字资源
		return ;
	}
	else
	{
		MessageBoxW(NULL, L" Client connect SERVER OK", L" Client connect SERVER OK", MB_OK);
	}




	/*while (true)
	{
		cin.getline(SendBuffer, sizeof(SendBuffer));
		Ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer), 0);
		if (Ret == SOCKET_ERROR)
		{
			cout << "Send Info Error::" << GetLastError() << endl;
			break;
		}
	}

	closesocket(ClientSocket);
	WSACleanup();
	return 0;
	*/
}
