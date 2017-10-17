#include <iostream>
#include <cstdlib>
#include <winsock2.h>
#include <string>
#include <time.h>
#include <Windows.h>
#include "UserMesInfo.h"
#include "stdafx.h"

#pragma comment(lib,"ws2_32.lib")

using namespace std;

UserInfo *gUserInfo;

char msgc[100][600];

char tmp[30] = { 0 };

void on_change_state(char* key)
{
	char* str1 = new char[20];
	char* str2 = new char[2];
	::CoInitialize(NULL);//初始化OLE/COM库环境，为访问ADO接口做准备
	_RecordsetPtr m_pRecordset("ADODB.Recordset");
	_ConnectionPtr m_pConnection("ADODB.Connection");
	_bstr_t bstrSQL("select * from user_information");
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");  //创建connection对象
		_bstr_t strConnect = "Provider=SQLOLEDB;Server=(local);Database=messagedb; uid=sa; pwd=password;";//设置连接字符串
		m_pConnection->Open(strConnect, "", "", adModeUnknown);
		if (m_pConnection == NULL)
			cout << "error! failed to connect sql server";
		m_pRecordset.CreateInstance(__uuidof(Recordset));//创建数据集对象
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		_variant_t userkey, userstate;
		while (!m_pRecordset->rsEOF)
		{
			userkey = m_pRecordset->GetCollect("user_key");
			userstate = m_pRecordset->GetCollect("user_state");
			strcpy(str1, (_bstr_t)userkey);
			strcpy(str2, (_bstr_t)userstate);
			if (strcmp(str1, key) == 0 && strcmp(str2, "0") == 0)
			{
				m_pRecordset->Fields->GetItem(_variant_t("user_state"))->PutValue((_bstr_t("1")));
				m_pRecordset->Update();
				break;
			}
			m_pRecordset->MoveNext();
		}
		//if(m_pRecordset->rsEOF){cout<<"该用户不存在"<<endl;return;}
		m_pRecordset->Close();
	}
	catch (_com_error e)
	{
		cerr << "\nERROR:" << (char*)e.Description();
	}
	::CoUninitialize();
}

int check_m(char* key)
{
	int num = 0;
	char* str = new char[20];
	::CoInitialize(NULL);//初始化OLE/COM库环境，为访问ADO接口做准备
	_RecordsetPtr m_pRecordset("ADODB.Recordset");
	_ConnectionPtr m_pConnection("ADODB.Connection");
	_bstr_t bstrSQL("select * from message_information");
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");  //创建connection对象
		_bstr_t strConnect = "Provider=SQLOLEDB;Server=(local);Database=messagedb; uid=sa; pwd=password;";//设置连接字符串
		m_pConnection->Open(strConnect, "", "", adModeUnknown);
		if (m_pConnection == NULL)
			cout << "error! failed to connect sql server";
		m_pRecordset.CreateInstance(__uuidof(Recordset));//创建数据集对象
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		_variant_t keyr;
		while (!m_pRecordset->rsEOF)
		{
			keyr = m_pRecordset->GetCollect("user_key_receive");
			strcpy(str, (_bstr_t)keyr);
			if (strcmp(str, key) == 0)num++;
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
	}
	catch (_com_error e)
	{
		cerr << "\nERROR:" << (char*)e.Description();
	}
	::CoUninitialize();
	return num;
}

void get_message(char* key)
{
	memset(msgc, 0, sizeof(msgc));
	int i = 0;
	char* str1 = new char[20];
	::CoInitialize(NULL);//初始化OLE/COM库环境，为访问ADO接口做准备
	_RecordsetPtr m_pRecordset("ADODB.Recordset");
	_ConnectionPtr m_pConnection("ADODB.Connection");
	_bstr_t bstrSQL("select * from message_information");
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");  //创建connection对象
		_bstr_t strConnect = "Provider=SQLOLEDB;Server=(local);Database=messagedb; uid=sa; pwd=password;";//设置连接字符串
		m_pConnection->Open(strConnect, "", "", adModeUnknown);
		if (m_pConnection == NULL)
			cout << "error! failed to connect sql server";
		m_pRecordset.CreateInstance(__uuidof(Recordset));//创建数据集对象
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		_variant_t keyr, msgcache;
		while (!m_pRecordset->rsEOF)
		{
			keyr = m_pRecordset->GetCollect("user_key_receive");
			strcpy(str1, (_bstr_t)keyr);
			//strcpy(str2,(_bstr_t)userstate);
			if (strcmp(str1, key) == 0)
			{
				msgcache = m_pRecordset->GetCollect("message_content");
				strcpy(msgc[i], (_bstr_t)msgcache);
				i++;
			}
			m_pRecordset->Delete(adAffectCurrent);
			m_pRecordset->Update();
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
	}
	catch (_com_error e)
	{
		cerr << "\nERROR:" << (char*)e.Description();
	}
	::CoUninitialize();

}

void off_change_state(char* key)
{
	char* str1 = new char[20];
	char* str2 = new char[2];
	::CoInitialize(NULL);//初始化OLE/COM库环境，为访问ADO接口做准备
	_RecordsetPtr m_pRecordset("ADODB.Recordset");
	_ConnectionPtr m_pConnection("ADODB.Connection");
	_bstr_t bstrSQL("select * from user_information");
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");  //创建connection对象
		_bstr_t strConnect = "Provider=SQLOLEDB;Server=(local);Database=messagedb; uid=sa; pwd=password;";//设置连接字符串
		m_pConnection->Open(strConnect, "", "", adModeUnknown);
		if (m_pConnection == NULL)
			cout << "error! failed to connect sql server";
		m_pRecordset.CreateInstance(__uuidof(Recordset));//创建数据集对象
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		_variant_t userkey, userstate;
		while (!m_pRecordset->rsEOF)
		{
			userkey = m_pRecordset->GetCollect("user_key");
			userstate = m_pRecordset->GetCollect("user_state");
			strcpy(str1, (_bstr_t)userkey);
			strcpy(str2, (_bstr_t)userstate);
			if (strcmp(str1, key) == 0 && strcmp(str2, "1") == 0)
			{
				m_pRecordset->Fields->GetItem(_variant_t("user_state"))->PutValue((_bstr_t("0")));
				m_pRecordset->Update();
				break;
			}
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
	}
	catch (_com_error e)
	{
		cerr << "\nERROR:" << (char*)e.Description();
	}
	::CoUninitialize();
}

char get_state(char* key)
{
	//cout<<"flag"<<key<<endl;
	char* str1 = new char[20];
	char* str2 = new char;
	::CoInitialize(NULL);//初始化OLE/COM库环境，为访问ADO接口做准备
	_RecordsetPtr m_pRecordset("ADODB.Recordset");
	_ConnectionPtr m_pConnection("ADODB.Connection");
	_bstr_t bstrSQL("select * from user_information");
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");  //创建connection对象
		_bstr_t strConnect = "Provider=SQLOLEDB;Server=(local);Database=messagedb; uid=sa; pwd=password;";//设置连接字符串
		m_pConnection->Open(strConnect, "", "", adModeUnknown);
		if (m_pConnection == NULL)
			cout << "error! failed to connect sql server";
		m_pRecordset.CreateInstance(__uuidof(Recordset));//创建数据集对象
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		_variant_t userkey, userstate;
		while (!m_pRecordset->rsEOF)
		{
			userkey = m_pRecordset->GetCollect("user_key");
			userstate = m_pRecordset->GetCollect("user_state");
			strcpy(str1, (_bstr_t)userkey);
			//strcpy(str2,(_bstr_t)userstate);
			if (strcmp(str1, key) == 0)
			{
				strcpy(str2, (_bstr_t)userstate);
				return (*str2);
			}
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
	}
	catch (_com_error e)
	{
		cerr << "\nERROR:" << (char*)e.Description();
	}
	::CoUninitialize();
	return '2';
}

void message_cache(char* ks, char* kr, char* tm, char* m)
{
	::CoInitialize(NULL);//初始化OLE/COM库环境，为访问ADO接口做准备
	_RecordsetPtr m_pRecordset("ADODB.Recordset");
	_ConnectionPtr m_pConnection("ADODB.Connection");
	_bstr_t bstrSQL("select * from message_information");
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");  //创建connection对象
		_bstr_t strConnect = "Provider=SQLOLEDB;Server=(local);Database=messagedb; uid=sa; pwd=password;";//设置连接字符串
		m_pConnection->Open(strConnect, "", "", adModeUnknown);
		if (m_pConnection == NULL)
			cout << "error! failed to connect sql server";
		m_pRecordset.CreateInstance(__uuidof(Recordset));//创建数据集对象
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		if (!m_pRecordset->Supports(adAddNew)) return;
		m_pRecordset->AddNew();
		m_pRecordset->Fields->GetItem(_variant_t("user_key_sent"))->Value = _bstr_t(ks);
		m_pRecordset->Fields->GetItem(_variant_t("user_key_receive"))->Value = _bstr_t(kr);
		m_pRecordset->Fields->GetItem(_variant_t("time_sent"))->Value = _bstr_t(tm);
		m_pRecordset->Fields->GetItem(_variant_t("message_content"))->Value = _bstr_t(m);
		m_pRecordset->Update();
		m_pRecordset->Close(); // 关闭记录集
	}
	catch (_com_error e)
	{
		cerr << "\nERROR:" << (char*)e.Description();
	}
	::CoUninitialize();
}
void sendmessage(char* toname, char*message)
{
	UserInfo* tempUser = gUserInfo;
	BOOL sendFlag = false;
	while (tempUser)
	{
		if (!memcmp(toname, tempUser->userName, strlen(toname)))
		{
			SOCKET toSock = tempUser->userSock;
			send(toSock, message, strlen(message) + 1, 0);
			cout <<toname<<":"<< message << endl;
			sendFlag = true;
		}
		tempUser = tempUser->pNext;
	}
	if (!sendFlag)
	{
		printf("短信丢失 !\n");
	}
	delete tempUser;
}

DWORD WINAPI ProcMes(LPVOID lpParameter)
{
	SOCKET cliSocket = ((UserInfo*)lpParameter)->userSock;
	char recvBuf[2000];
	while (true)
	{
		memset(recvBuf, 0x00, 2000);
		recv(cliSocket, recvBuf, 2000, 0);
		time_t t = time(0);
		strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
		short MesType = *(short*)recvBuf;

		switch (MesType)
		{
		case MesLogin:
		{
						 static short USERID = 1001;
						 UserInfo* userInfo = new UserInfo;
						 char* userName = (char*)(recvBuf + 2);
						 strcpy(userInfo->userName, userName);
						 userInfo->userId = USERID++;
						 userInfo->userSock = cliSocket;
						 userInfo->userAddr = ((UserInfo*)lpParameter)->userAddr;
						 userInfo->pNext = NULL;

						 if (!gUserInfo)
						 {
							 gUserInfo = userInfo;
						 }
						 else
						 {
							 userInfo->pNext = gUserInfo;
							 gUserInfo = userInfo;
						 }
						 printf("Login  :  %s ==> %d !\n", userInfo->userName, userInfo->userId);

						 char loginInfo[4] = { 0 };
						 memcpy(loginInfo, &LoginSuc, 2);

						 int i = send(cliSocket, loginInfo, strlen(loginInfo), 0);
						 on_change_state(userInfo->userName);
						 int mesnum = check_m(userInfo->userName);
						 //cout << mesnum << endl;
						 if(mesnum>0)
						 {
							 get_message(userInfo->userName);
							 int iii=0;
							 while (iii<mesnum)
							 {
								 char SendMes[2000] = { 0 };
								 memcpy(SendMes, &MesNormal, 2);
								 memcpy(SendMes+2, msgc[iii], strlen(msgc[iii]) + 1);
								 cout <<"延时:\n"<< SendMes << endl;
								 send(userInfo->userSock, SendMes,strlen(SendMes)+1, 0);
								 //cout << msgc[iii] << endl;
								 cout<<"延时短信已发送！"<<endl;
								 Sleep(100);
								 
								 time_t t = time(0);
								 strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
								 char fromname[32] = { 0 };
								 char Messback[1000] = { 0 };
								 memcpy(fromname,msgc[iii],11);
								 memcpy(Messback,&MesNormal,2);
								 memcpy(Messback + 2, "55555555555:\n",13);
								 strcat(Messback, tmp);
								 strcat(Messback, "\n");
								 strcat(Messback, "您发送的短信延时后已成功被");
								 strcat(Messback, userInfo->userName);
								 strcat(Messback, "接收！");
								 //cout << Messback << endl;
								 sendmessage(fromname,Messback);
								 
								 iii++;
								 
							 }		
						 }			
		}
			break;
		case MesNormal:
		{
						  char toName[32] = { 0 };
						  memcpy(toName, recvBuf + 2, 32);
						 // cout << toName << endl;
						  char Message[2000] = { 0 };
						  memcpy(Message, recvBuf + 34, sizeof(recvBuf)-34);
						 // cout << Message << endl;

						  char sendMes[2000];
						  memcpy(sendMes, &MesNormal, 2);
						  memcpy(sendMes + 2, recvBuf + 34, sizeof(recvBuf)-34);
						  char state = get_state(toName);
						  //cout<<state<<endl;
						  if (state == '1')
						  {
							  sendmessage(toName, sendMes);
							  cout << "成功发送！" << endl;
							  
							  time_t t = time(0);
							  strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));

							  char fromname[32] = { 0 };
							  char Messback[1000] = { 0 };
							  memcpy(fromname, Message, 11);
							  memcpy(Messback, &MesNormal, 2);
							  memcpy(Messback + 2,"55555555555:\n",13);
							  strcat(Messback, tmp);
							  strcat(Messback, "\n");
							  strcat(Messback, "您发送的短信已成功被");
							  strcat(Messback, toName);
							  strcat(Messback, "接收！");
							  //cout << Messback << endl;
							  sendmessage(fromname, Messback);
							  
						  }
						  else if (state == '0')
						  {
							  char fromname[32] = { 0 };
							  
							  time_t t = time(0);
							  strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));

							  char messback[1000] = { 0 };
							  memcpy(messback, &MesNormal, 2);
							  memcpy(messback + 2, "55555555555:\n", 13);
							  strcat(messback, tmp);
							  strcat(messback, "\n");
							  strcat(messback, toName);
							  strcat(messback, "已关机，短信暂时无法发送！");
							  //cout << messback << endl;
							  sendmessage(fromname, messback);
							  
							  
							  memcpy(fromname, Message, 11);
							  char sendtime[20] = { 0 };
							  memcpy(sendtime, Message + 13, 19);
							  char sendmess[2000] = { 0 };
							  memcpy(sendmess, Message, strlen(Message) + 1);
							  message_cache(fromname, toName, sendtime, sendmess);

							 
						  }
		}
			break;
		case MesTime:
		{
						  Sleep(40000);
						  char toName[32] = { 0 };
						  memcpy(toName, recvBuf + 2, 32);
						  cout << toName << endl;
						  char Message[2000] = { 0 };
						  memcpy(Message, recvBuf + 34, sizeof(recvBuf)-34);
						  cout << Message << endl;

						  char sendMes[2000];
						  memcpy(sendMes, &MesNormal, 2);
						  memcpy(sendMes + 2, recvBuf + 34, sizeof(recvBuf)-34);
						  char state = get_state(toName);
						  //cout<<state<<endl;
						  if (state == '1')
						  {
							  sendmessage(toName, sendMes);
							  cout << "成功发送！" << endl;
							  
							  time_t t = time(0);
							  strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));

							  char fromname[32] = { 0 };
							  char Messback[1000] = { 0 };
							  memcpy(fromname, Message, 11);
							  memcpy(Messback, &MesNormal, 2);
							  memcpy(Messback + 2, "55555555555:\n", 13);
							  strcat(Messback, tmp);
							  strcat(Messback, "\n");
							  strcat(Messback, "您发送的短信已成功被");
							  strcat(Messback, toName);
							  strcat(Messback, "接收！");
							  //cout << Messback << endl;
							  sendmessage(fromname, Messback);
							  
						  }
						  else if (state == '0')
						  {
							  char fromname[32] = { 0 };
							  memcpy(fromname, Message, 11);
							  char sendtime[20] = { 0 };
							  memcpy(sendtime, Message + 13, 19);
							  char sendmess[2000] = { 0 };
							  memcpy(sendmess, Message, strlen(Message) + 1);
							  message_cache(fromname, toName, sendtime, sendmess);
							  
							  time_t t = time(0);
							  strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));

							  char messback[1000] = { 0 };
							  memcpy(messback, &MesNormal, 2);
							  memcpy(messback + 2, "55555555555:\n", 13);
							  strcat(messback, tmp);
							  strcat(messback, "\n");
							  strcat(messback, toName);
							  strcat(messback, "已关机，短信暂时无法发送！");
							  //cout << messback << endl;
							  sendmessage(fromname, messback);
							  
						  }
		}
			break;
		case MesLogout:
			{
				char username[32] = { 0 };
				strcpy(username,recvBuf+2);
				off_change_state(username);
				cout << username << "log out!" << endl;
			}
			break;
		default:
			break;
		}

	}


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


	SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(5876);
	addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	bind(servSock, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));

	if (listen(servSock, 20) == SOCKET_ERROR)
	{
		printf("listen failed with error: %ld\n", WSAGetLastError());
		closesocket(servSock);
		WSACleanup();
		return -1;
	}


	while (true)
	{
		//wait for users Login, and create thread for it
		SOCKADDR_IN cliAddr;
		int length = sizeof(SOCKADDR);

		SOCKET cliSock = accept(servSock, (SOCKADDR*)&cliAddr, &length);

		if (INVALID_SOCKET == cliSock)
		{
			printf("listen failed with error: %ld\n", WSAGetLastError());
			closesocket(cliSock);
			WSACleanup();
			return -1;
		}

		UserInfo* user = new UserInfo;

		user->userSock = cliSock;
		user->userAddr = cliAddr.sin_addr.S_un.S_addr;

		//cout<<sizeof(user->userName)<<endl;

		HANDLE loginHandle = CreateThread(NULL, 0, ProcMes, (LPVOID)user, 0, NULL);

		if (NULL == loginHandle)
		{
			cout << "Create Thread failed !" << endl;
			return -1;
		}
		CloseHandle(loginHandle);

	}
	//Sleep(20000);
	closesocket(servSock);
	WSACleanup();

	system("PAUSE");
	return 0;
}