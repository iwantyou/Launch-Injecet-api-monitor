// Server.cpp: �������̨Ӧ�ó������ڵ㡣
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

	WSADATA            wsad;            //WSADATA����
	SOCKET            Server;        //�������׽���
	SOCKET            Client;        //�ͻ����׽���
	SOCKADDR_IN        addrServ;        //��������ַ
	char            buf[BUFSIZE];        //�������ݻ�����
	int                retVal;        //����ֵ

	ofile.open("D:\\API_logs.log");
									  //��ʼ���׽��ֶ�̬��
	if (WSAStartup(MAKEWORD(2, 2), &wsad) != 0)
	{
		printf("��ʼ���׽��ֶ�̬��ʧ��!\n");
		return 1;
	}

	//�����׽���
	Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == Server)
	{
		printf("�����׽���ʧ��!\n");
		WSACleanup();//�ͷ��׽�����Դ;
		return  -1;
	}

	//�������׽��ֵ�ַ 
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(4000);
	addrServ.sin_addr.s_addr = INADDR_ANY;


	//���׽���
	retVal = bind(Server, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		printf("���׽���ʧ��!\n");
		closesocket(Server);    //�ر��׽���
		WSACleanup();            //�ͷ��׽�����Դ;
		return -1;
	}

	//��ʼ���� 
	retVal = listen(Server, 1);
	if (SOCKET_ERROR == retVal)
	{
		printf("����ʧ��!\n");
		closesocket(Server);    //�ر��׽���
		WSACleanup();            //�ͷ��׽�����Դ;
		return -1;
	}

	//���ܿͻ�������
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	printf("�ȴ����ܿͻ�������!\n");
	Client = accept(Server, (sockaddr FAR*)&addrClient, &addrClientlen);
	if (INVALID_SOCKET == Client)
	{
		printf("���ܿͻ�������ʧ��!\n");
		closesocket(Server);    //�ر��׽���
		WSACleanup();            //�ͷ��׽�����Դ;
		return -1;
	}
	else
	{
		printf("���ܿͻ��˳ɹ�!\n");
	}

	//���տͻ�������

	while (true)
	{
		printf("��socket�����\n");
		ZeroMemory(buf, BUFSIZE);
		retVal = recv(Client, buf, BUFSIZE, 0);
		if (SOCKET_ERROR == retVal)
		{
			printf("���տͻ�������ʧ��!\n");
			closesocket(Server);    //�ر��׽���
			closesocket(Client);    //�ر��׽���        
			WSACleanup();            //�ͷ��׽�����Դ;
			return -1;
		}
		//�����log
		LOG(INFO) << buf << std::endl;
		//ofile << buf;
		//std::cout << buf << std::endl;   //�������Client���ַ���    
	}


	//�˳�
	closesocket(Server);    //�ر��׽���
	closesocket(Client);    //�ر��׽���
	WSACleanup();            //�ͷ��׽�����Դ;
	return 0;
}

