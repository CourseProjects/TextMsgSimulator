#ifndef  User_Mes_Sock
#define  User_Mes_Sock
#include <winsock2.h>
#endif

const short  MesLogin = 1001;
const short  MesLogout = 1002;
const short  MesNormal = 1003;
const short  LoginSuc = 1004;
const short  MesTime = 1005;
const short  MesBack = 1006;
const short  OnLineUser = 1010;


//User Struct
struct UserInfo
{
	short        userId;
	char         userName[32];
	SOCKET       userSock;
	LONG         userAddr;
	UserInfo     *pNext;
};

//Message Struct
struct MesInfo
{
	short        MesType;
	char         fromUserName[32];
	char         toUserName[32];
	char         MesContent[2000];
};