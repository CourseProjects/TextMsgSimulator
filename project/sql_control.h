#include<iostream>
#include"stdafx.h"
#include<string>
using namespace std;

char msgc[100][600];


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