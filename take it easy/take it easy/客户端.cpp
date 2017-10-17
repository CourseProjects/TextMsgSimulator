#include <iostream>
#include <stdlib.h>
#include <WinSock2.h>

#include "UserMesInfo.h"

#pragma comment(lib,"ws2_32.lib")

using namespace std;

BOOL isLogin = false;


string charToString(char temp[])
{
	string tempStr = "";
	for (int i = 0; i < sizeof(temp); i++)
		tempStr += temp[i];
	return tempStr;
}

BOOL SendMes(SOCKET servSocket)
{
	SOCKET servSock = servSocket;
	while (TRUE)
	{
		int tempWho;
		char sendBuff[1024] = { 0 };
		if (isLogin)
		{
			printf(" '1' : speak to ONE ;\n '2' : speak to ALL ;\n '3' : OnLine Users ; \n '4' : Exit ;\n");

			cin >> tempWho;
		}
		if (isLogin && tempWho == 1)
		{
			printf("Please input NAME you want to CHAT : ");
			char toName[32] = { 0 };
			scanf("%s", toName);
			memcpy(sendBuff, &MesNormal, 2);
			memcpy(sendBuff + 2, toName, 32);
			printf("Please input your name:");
			char myName[1000] = { 0 };
			scanf("%s", myName);
			//memcpy(sendBuff+34,myName,32 );
			printf("Please input what do you want to say : ");
			char Message[1000] = { 0 };
			scanf("%s", Message);
			strcat(myName, Message);
			memcpy(sendBuff + 34,myName, strlen(myName)+1);
			send(servSock, sendBuff, sizeof(sendBuff)+1, 0);
		}
		/*else if (isLogin && tempWho == 2){
			//To ALL
			printf("Please input DATA : ");
			char Message[1000] = { 0 };
			scanf("%s", Message);

			memcpy(sendBuff, &MesToAll, 2);
			memcpy(sendBuff + 2, Message, strlen(Message));
			send(servSock, sendBuff, sizeof(sendBuff)+1, 0);

		}
		else if (isLogin && tempWho == 3){
			char getOnLineUsers[4];
			memcpy(getOnLineUsers, &OnLineUser, 2);
			send(servSock, getOnLineUsers, sizeof(getOnLineUsers), 0);
		}*/
		else if (isLogin && tempWho == 4){
			break;
		}
		else if (isLogin){
			printf("You input Error! Please Again !");
		}

	}
	return TRUE;
}


DWORD WINAPI ProcThread(LPVOID lpParameter)
{
	SOCKET cliSocket = *(SOCKET*)lpParameter;

	//SOCKADDR_IN sockFrom ;
	//int length = sizeof(SOCKADDR);

	char recvBuf[1024] = { 0 };

	while (true)
	{
		memset(recvBuf, 0x00, 1024);
		int recvLen = recv(cliSocket, recvBuf, 1024, 0);
		if (recvLen > 0)
		{
			short megId = *(short*)recvBuf;
			switch (megId)
			{
			case LoginSuc:
			{
							 //char userName[32] = {0};
							 //memcpy(userName, recvBuf+2, 4);
							 printf("Login Success !\n");
							 isLogin = true;
			}
				break;
			case MesNormal:
			{
							  char Mess[1000];
							  memcpy(Mess, recvBuf + 2, sizeof(recvBuf)-2);
							  printf(" ==> Me :%s\n", Mess);

			}
				break;
			case MesToAll:
			{
							 char Mess[1000];
							 memcpy(Mess, recvBuf + 2, sizeof(recvBuf)-2);
							 printf(" ==> All :%s\n", Mess);
			}
				break;
			case OnLineUser:
			{
							   char onLineUser[100] = { 0 };

							   memcpy(onLineUser, recvBuf + 2, sizeof(recvBuf)-2);

							   //string userStr = charToString(onLineUser);

							   for (int i = 0; i < sizeof(onLineUser); i++)
								   cout << onLineUser[i];

							   //printf("%s\n", userStr);
							   cout << endl << onLineUser << endl;
							   printf("\n");

			}
				break;
			default:
				break;
			}
		}//end if
	}// end while

	return 0;
}

int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		printf("WSAStartup failed with error: %d\n", err);
		return 1;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		printf("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
		return 1;
	}
	else
		printf("The Winsock 2.2 dll was found okay\n");


	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, 0);

	if (clientSock == -1)
	{
		perror("socket创建出错！");
		exit(1);
	}

	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(6000);


	int ret = connect(clientSock, (sockaddr*)&addrClient, sizeof(SOCKADDR));
	if (ret)
	{
		printf("连接失败\n");
		return -1;
	}

	HANDLE thHandle = CreateThread(NULL, 0, ProcThread, &clientSock, 0, NULL);

	if (NULL == thHandle)
	{
		cout << "Create Thread failed !" << endl;
		return -1;
	}

	char userName[32];
	printf("Please input name : ");
	scanf_s("%s", userName, 31);

	/*
	UserInfo* user = new UserInfo;
	user->userSock = clientSock;
	strcpy_s(user->userName, userName);
	user->userAddr = addrClient.sin_addr.S_un.S_addr;
	*/

	char loginInfo[64] = { 0 };
	memcpy(loginInfo, &MesLogin, 2);
	memcpy(loginInfo + 2, userName, strlen(userName));


	//int a = send(clientSock, loginInfo, strlen(loginInfo), 0);
	int a = send(clientSock, loginInfo, strlen(loginInfo), 0);

	SendMes(clientSock);

	//Sleep(20000);
	//closesocket(clientSock);
	CloseHandle(thHandle);
	WSACleanup();


	system("PAUSE");
	return 0;
}