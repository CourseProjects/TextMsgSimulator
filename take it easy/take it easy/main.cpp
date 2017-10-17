#include <iostream>
#include <stdlib.h>
#include <WinSock2.h>
#include "UserMesInfo.h"
#include <gtk/gtk.h>
#include <time.h> 
#include <stdio.h> 
#include<string>
#include "sqlite3.h" 
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"sqlite3.lib")


char* _(char* str) { return g_locale_to_utf8(str, -1, 0, 0, 0); }


char locktime[30] = { 0 };

sqlite3 *db = NULL;
int row; //短信条数
char msg[100][200]; //短信暂存二维数组
char ph_num[20]; //联系人手机号
char name[20];  //联系人姓名
BOOL isLogin = false;
SOCKET clientSock;
char server_ip[20];//服务器ip
int using_port;//使用的端口号
char* login_buff; //登陆手机号
char* to_ph_num;  //收信人手机号
char* to_name;//收信人姓名
char* text_mess;//发送的短信内容
char t_mess[100];//长短信切割后发送的短信内容
char rece_mess[400]; //收到的短信
int s_len; //长短信长度
int i = 0;
int j = 0;
int circ = 0; //长短信循环圈数
char* const_time;  //定时时间
char tmp[64];   //时间含秒
char short_tmp[20]; //时间，不含秒
char sendBuff[2000];//向服务器发送的短信内容
short megId;//识别信号
int nxt;//删除短信时“下一批”次数
int recvLen;
char recvBuf[1024];
time_t t = time(0);

GtkWidget *window;
GtkWidget *fixed0;
GtkWidget *button01;
GtkWidget *button02;
GtkWidget *label;
GtkWidget *entry;
GtkWidget *image;

GtkWidget *window1;
GtkWidget *scrolled1;
GtkWidget *view1;
GtkWidget *button11;
GtkWidget *button12;
GtkWidget *button13;
GtkWidget *button14;
GtkWidget *button15;
GtkWidget *button16;
GtkWidget *button17;
GtkWidget *button18;
GtkWidget *button19;
GtkWidget *button161;
GtkWidget *button162;
GtkWidget *button163;
GtkWidget *fixed11;
GtkWidget *entry1;
GtkWidget *vbox11;

GtkWidget *window2;
GtkWidget *fixed2;
GtkWidget *button21;
GtkWidget *button22;
GtkWidget *button23;
GtkWidget *check21;
GtkWidget *check22;
GtkWidget *check23;
GtkWidget *check24;
GtkWidget *check25;
GtkWidget *label21;
GtkWidget *vbox21;

GtkWidget *window3;
GtkWidget *scrolled3;
GtkWidget *view3;
GtkWidget *fixed3;
GtkWidget *button31;
GtkWidget *button32;
GtkWidget *button33;
GtkWidget *button34;
GtkWidget *entry31;
GtkWidget *entry32;
GtkWidget *combo3;
GtkWidget *label31;
GtkWidget *label32;
GtkWidget *label33;
GtkWidget *label34;
GtkWidget *radio_button1;
GtkWidget *radio_button2;
GSList *radio_group;

GtkWidget *window4;
GtkWidget *fixed41;
GtkWidget *fixed42;
GtkWidget *button41;
GtkWidget *button42;
GtkWidget *check41;
GtkWidget *check42;
GtkWidget *check43;
GtkWidget *check44;
GtkWidget *check45;
GtkWidget *label41;
GtkWidget *label42;
GtkWidget *scrolled41;
GtkWidget *view41;
GtkWidget *scrolled42;

GtkWidget *window5;
GtkWidget *scrolled5;
GtkWidget *view5;
GtkWidget *fixed5;
GtkWidget *button51;
GtkWidget *button52;
GtkWidget *button53;
GtkWidget *button54;
GtkWidget *button55;
GtkWidget *entry51;
GtkWidget *entry52;
GtkWidget *entry53;
GtkWidget *combo5;
GtkWidget *label51;
GtkWidget *label52;
GtkWidget *label53;
GtkWidget *label54;
GtkWidget *label55;
GtkWidget *label56;
GtkWidget *label57;
GtkWidget *radio_button51;
GtkWidget *radio_button52;
GSList *radio_group5;

GtkWidget *window6;
GtkWidget *scrolled61;
GtkWidget *scrolled62;
GtkWidget *view6;
GtkWidget *fixed6;
GtkWidget *button61;
GtkWidget *button62;
GtkWidget *button63;
GtkWidget *button64;
GtkWidget *label61;
GtkWidget *label62;
GtkWidget *label63;
GtkWidget *label64;
GtkWidget *vbox61;

GtkWidget *window7;
GtkWidget *fixed7;
GtkWidget *button71;
GtkWidget *button72;
GtkWidget *button73;
GtkWidget *check71;
GtkWidget *check72;
GtkWidget *check73;
GtkWidget *check74;
GtkWidget *check75;
GtkWidget *label71;
GtkWidget *vbox71;

void show0();
void show();
void show1();
void show2();
void show3();
void show4();
void show5();
void show6();
void time_send();

void des_show0(GtkWidget* widget);
void des_show(GtkWidget* widget);
void des_show1(GtkWidget* widget);
void des_show2(GtkWidget* widget);
void des_show3(GtkWidget* widget);
void des_show4(GtkWidget* widget);
void des_show5(GtkWidget* widget);
void des_show6(GtkWidget* widget);

void show_cont1(GtkWidget *widget);
void show_cont2(GtkWidget *widget);
void show_cont3(GtkWidget *widget);
void show_cont4(GtkWidget *widget);
void show_cont5(GtkWidget *widget);
void send_mes(); 
void send_mes1();
void send_mes2();
BOOL Logout(SOCKET logSocket, char*logoutname);


void opendb()
{
	int rt;
	rt = sqlite3_open("message_client.db", &db); //打开数据库

	//如果连接成功，则返回SQLITE_OK，其值为0；如果失败则返回大于0的值
	if (rt)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);	//注意，这里也需要调用sqlite3_close
	}
}
void name2ph_num(char* n)
{
	//ANSItoUTF8(n);
	memset(ph_num, 0, sizeof(ph_num));
	int i, j, column, index, result, r1, sum;
	char *zErrMsg = NULL;
	char** dbresult;
	string sqlstr;
	sqlstr = "SELECT friend_key FROM contactor WHERE friend_name ='";
	sqlstr += n;
	sqlstr += "';";
	result = sqlite3_get_table(db, sqlstr.c_str(), &dbresult, &r1, &column, &zErrMsg);
	cout << r1 << "  " << column << endl;
	if (SQLITE_OK == result)
	{
		index = column;
		sum = 0;
		for (i = 0; i<r1; i++)
		{
			for (j = 0; j<column; j++)
			{
				strcpy(ph_num, dbresult[column]);
				sqlite3_free_table(dbresult);
				index++;
				sum++;
			}
		}
	}
}
void select_m(char* uk, char* fk)
{
	memset(msg, 0, sizeof(msg));
	int i, j, column, index, result, sum;
	char *zErrMsg = NULL;
	string sqlstr;
	sqlstr = "SELECT message_content FROM short_message WHERE user_key =";
	sqlstr += "'";
	sqlstr += uk;
	sqlstr += "' AND friend_key = '";
	sqlstr += fk;
	sqlstr += "';";
	char** dbresult;
	result = sqlite3_get_table(db, sqlstr.c_str(), &dbresult, &row, &column, &zErrMsg);
	//cout << row << "   " << column << endl;
	if (SQLITE_OK == result)
	{
		index = column;
		sum = 0;
		for (i = 0; i<row; i++)
		{
			for (j = 0; j<column; j++)
			{
				strcpy(msg[sum], dbresult[index]);
				++index;
				sum++;
			}
		}
		sqlite3_free_table(dbresult);
	}
}
void delete_m1(char* uk, char* fk)
{
	int rd;
	string sqlstr;
	char* zErrMsg = NULL;
	sqlstr = "DELETE FROM short_message WHERE user_key = '";
	sqlstr += uk;
	sqlstr += "' AND friend_key = '";
	sqlstr += fk;
	sqlstr += "';";
	rd = sqlite3_exec(db, sqlstr.c_str(), NULL, 0, &zErrMsg);
	if (rd != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);  //注意这里对 zErrMsg 的释放
	}
}
void delete_m2(char* uk, char* fk, char* mc)
{
	int rd;
	string sqlstr;
	char* zErrMsg = NULL;
	sqlstr = "DELETE FROM short_message WHERE user_key = '";
	sqlstr += uk;
	sqlstr += "' AND friend_key='";
	sqlstr += fk;
	sqlstr += "' AND message_content = '";
	sqlstr += mc;
	sqlstr += "';";
	rd = sqlite3_exec(db, sqlstr.c_str(), NULL, 0, &zErrMsg);
	if (rd != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);  //注意这里对 zErrMsg 的释放
	}
}
void receive_insert(char* uk, char* ks, char* mc, char* tm)
{
	int rr;
	char *zErrMsg = NULL;
	string sqlstr;
	sqlstr = "INSERT INTO short_message VALUES('";
	sqlstr += uk;
	sqlstr += "','";
	sqlstr += ks;
	sqlstr += "','";
	sqlstr += tm;
	sqlstr += "','";
	sqlstr += mc;
	sqlstr += "',1);";
	rr = sqlite3_exec(db, sqlstr.c_str(), NULL, 0, &zErrMsg);
	if (rr != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);  //注意这里对 zErrMsg 的释放
	}
}
void sent_insert(char* uk, char* kr, char* mc, char* tm)
{
	int rs;
	char *zErrMsg = NULL;
	string sqlstr;
	sqlstr = "INSERT INTO short_message VALUES('";
	sqlstr += uk;
	sqlstr += "','";
	sqlstr += kr;
	sqlstr += "','";
	sqlstr += tm;
	sqlstr += "','";
	sqlstr += mc;
	sqlstr += "',2);";
	rs = sqlite3_exec(db, sqlstr.c_str(), NULL, 0, &zErrMsg);
	if (rs != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);  //注意这里对 zErrMsg 的释放
	}
}
void show_info1()
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(NULL,
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_INFO,
		GTK_BUTTONS_OK,
		_("短信已发送给服务器"));
	gtk_window_set_title(GTK_WINDOW(dialog), _("发送短信"));
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
void show_info2()
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(NULL,
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_INFO,
		GTK_BUTTONS_OK,
		_("已选联系人所有短信已删除"));
	gtk_window_set_title(GTK_WINDOW(dialog), _("删除所有短信"));
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
void show_info3()
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(NULL,
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_INFO,
		GTK_BUTTONS_OK,
		_("已选短信已删除"));
	gtk_window_set_title(GTK_WINDOW(dialog), _("选择删除短信"));
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
void unread_mess()
{

	GtkWidget *window_mess;
	window_mess = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window_mess), _("未读短信"));
	gtk_window_set_default_size(GTK_WINDOW(window_mess), 430, 550);
	gtk_window_set_position(GTK_WINDOW(window_mess), GTK_WIN_POS_CENTER);


	GtkWidget *fixed_mess;
	fixed_mess = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window_mess), fixed_mess);

	GtkWidget *label_mess;
	label_mess = gtk_label_new(_(rece_mess));
	gtk_label_set_justify((GtkLabel*)label_mess, GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap((GtkLabel*)label_mess, TRUE);
	gtk_fixed_put(GTK_FIXED(fixed_mess), label_mess, 30, 70);

	GtkWidget *button_mess1;
	GtkWidget *button_mess2;
	button_mess1 = gtk_button_new_with_label(_("回复"));
	gtk_fixed_put(GTK_FIXED(fixed_mess), button_mess1, 150, 410);
	gtk_widget_set_size_request(button_mess1, 80, 35);
	g_signal_connect(G_OBJECT(button_mess1), "clicked",
		G_CALLBACK(des_show5), G_OBJECT(window_mess));

	button_mess2 = gtk_button_new_with_label(_("关闭"));
	gtk_fixed_put(GTK_FIXED(fixed_mess), button_mess2, 250, 410);
	gtk_widget_set_size_request(button_mess2, 80, 35);
	g_signal_connect_swapped(G_OBJECT(button_mess2), "clicked",
		G_CALLBACK(gtk_widget_destroy), G_OBJECT(window_mess));

	gtk_widget_show_all(window_mess);
	//g_print("sdfjkljgoiwijoiwe\n");
}

void get_time()
{
	gtk_widget_destroy(label56);
	gtk_widget_destroy(label57);

	time_t t = time(0);
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
	strcpy(short_tmp, tmp);
	short_tmp[16]='\0';

	label56 = gtk_label_new(_("当前系统时间为："));
	gtk_fixed_put(GTK_FIXED(fixed5), label56, 200, 190);
	gtk_widget_show(label56);
	label57 = gtk_label_new(_(short_tmp));
	gtk_fixed_put(GTK_FIXED(fixed5), label57, 200, 220);
	gtk_widget_show(label57);
}

void show_cont1(GtkWidget *widget)
{
	gtk_widget_destroy(widget);
	strcpy(name, "no1");
	show5();
}
void show_cont2(GtkWidget *widget)
{
	gtk_widget_destroy(widget);
	strcpy(name, "no2");
	show5();
}
void show_cont3(GtkWidget *widget)
{
	gtk_widget_destroy(widget);
	strcpy(name, "no3");
	show5();
}
void show_cont4(GtkWidget *widget)
{
	gtk_widget_destroy(widget);
	strcpy(name, "no4");
	show5();
}
void show_cont5(GtkWidget *widget)
{
	gtk_widget_destroy(widget);
	strcpy(name, "server");
	show5();
}

void des_show0(GtkWidget* widget)
{
	gtk_widget_destroy(widget);
	Logout(clientSock, login_buff);
	gtk_main_quit();
}
void des_show(GtkWidget* widget)
{
	gtk_widget_destroy(widget);
	show();
}
void des_show1(GtkWidget* widget)
{
	gtk_widget_destroy(widget);
	show1();
}
void des_show2(GtkWidget* widget)
{
	gtk_widget_destroy(widget);
	show2();
}
void des_show3(GtkWidget* widget)
{
	gtk_widget_destroy(widget);
	show3();
}
void des_show4(GtkWidget* widget)
{
	gtk_widget_destroy(widget);
	show4();
}
void des_show5(GtkWidget* widget)
{
	gtk_widget_destroy(widget);
	show5();
}
void des_show6(GtkWidget* widget)
{
	nxt = 0;
	gtk_widget_destroy(widget);
	show6();
}

GdkPixbuf *create_pixbuf(const gchar * filename)
{
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if (!pixbuf) {
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}

	return pixbuf;
}

void show0()
{
	gtk_widget_show(window);
}
void show()
{
	//window1
	window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window1), _("联系人"));
	gtk_window_set_default_size(GTK_WINDOW(window1), 430, 550);
	gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_MOUSE);
	gtk_window_set_icon(GTK_WINDOW(window1), create_pixbuf("1.ico"));
	//固定窗口大小
	gtk_widget_set_size_request(window1, 430, 550);
	gtk_window_set_resizable(GTK_WINDOW(window1), FALSE);

	fixed11 = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window1), fixed11);

	/*image = gtk_image_new_from_file("background.jpg");
	gtk_container_add(GTK_CONTAINER(fixed11), image);*/

	scrolled1 = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy((GtkScrolledWindow *)scrolled1, GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_widget_set_size_request(scrolled1, 330, 350);
	gtk_fixed_put(GTK_FIXED(fixed11), scrolled1, 30, 30);

	vbox11 = gtk_vbox_new(TRUE, 1);
	gtk_scrolled_window_add_with_viewport((GtkScrolledWindow *)scrolled1, (GtkWidget *)vbox11);
	//gtk_fixed_put(GTK_FIXED(fixed11), fixed12, 30, 100);

	/*image = gtk_image_new_from_file("background2.jpg");
	gtk_container_add(GTK_CONTAINER(vbox11), image);*/
	
	button15 = gtk_button_new_with_label(_("no1"));
	gtk_box_pack_start(GTK_BOX(vbox11), button15, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button15, 280, 35);

	button16 = gtk_button_new_with_label(_("no2"));
	gtk_box_pack_start(GTK_BOX(vbox11), button16, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button16, 280, 35);

	button161 = gtk_button_new_with_label(_("no3"));
	gtk_box_pack_start(GTK_BOX(vbox11), button161, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button161, 280, 35);

	button162 = gtk_button_new_with_label(_("no4"));
	gtk_box_pack_start(GTK_BOX(vbox11), button162, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button162, 280, 35);

	button163 = gtk_button_new_with_label(_("server"));
	gtk_box_pack_start(GTK_BOX(vbox11), button163, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button163, 280, 35);

	/*view1 = gtk_text_view_new();
	gtk_container_add(GTK_CONTAINER(scrolled1), view1);*/

	button11 = gtk_button_new_with_label(_("删除短信"));
	gtk_fixed_put(GTK_FIXED(fixed11), button11, 330, 500);
	gtk_widget_set_size_request(button11, 80, 35);

	button12 = gtk_button_new_with_label(_("即时短信"));
	gtk_fixed_put(GTK_FIXED(fixed11), button12, 180, 500);
	gtk_widget_set_size_request(button12, 80, 35);

	/*button13 = gtk_button_new_with_label(_("查找联系人"));
	gtk_fixed_put(GTK_FIXED(fixed11), button13, 345, 10);
	gtk_widget_set_size_request(button13, 80, 35);*/

	button14 = gtk_button_new_with_label(_("退出登录"));
	gtk_fixed_put(GTK_FIXED(fixed11), button14, 30, 500);
	gtk_widget_set_size_request(button14, 80, 35);

	/*entry1 = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed11), entry1, 10, 10);
	gtk_widget_set_size_request(entry1, 330, 35);*/

	/*button17 = gtk_button_new_with_label(_("长短信"));
	gtk_fixed_put(GTK_FIXED(fixed11), button17, 30, 450);
	gtk_widget_set_size_request(button17, 80, 35);*/

	/*button18 = gtk_button_new_with_label(_("群发短信"));
	gtk_fixed_put(GTK_FIXED(fixed11), button18, 180, 450);
	gtk_widget_set_size_request(button18, 80, 35);*/

	button19 = gtk_button_new_with_label(_("定时短信"));
	gtk_fixed_put(GTK_FIXED(fixed11), button19, 180, 450);
	gtk_widget_set_size_request(button19, 80, 35);

	g_signal_connect_swapped(G_OBJECT(button11), "clicked",
		G_CALLBACK(des_show1), G_OBJECT(window1));

	g_signal_connect_swapped(G_OBJECT(button12), "clicked",
		G_CALLBACK(des_show2), G_OBJECT(window1));

	g_signal_connect_swapped(G_OBJECT(button14), "clicked",
		G_CALLBACK(des_show0), G_OBJECT(window1));

	g_signal_connect_swapped(G_OBJECT(button18), "clicked",
		G_CALLBACK(des_show3), G_OBJECT(window1));

	g_signal_connect_swapped(G_OBJECT(button19), "clicked",
		G_CALLBACK(des_show4), G_OBJECT(window1));

	g_signal_connect_swapped(G_OBJECT(button15), "clicked",
		G_CALLBACK(show_cont1), G_OBJECT(window1));
	g_signal_connect_swapped(G_OBJECT(button16), "clicked",
		G_CALLBACK(show_cont2), G_OBJECT(window1));
	g_signal_connect_swapped(G_OBJECT(button161), "clicked",
		G_CALLBACK(show_cont3), G_OBJECT(window1));
	g_signal_connect_swapped(G_OBJECT(button162), "clicked",
		G_CALLBACK(show_cont4), G_OBJECT(window1));
	g_signal_connect_swapped(G_OBJECT(button163), "clicked",
		G_CALLBACK(show_cont5), G_OBJECT(window1));

	gtk_widget_show_all(window1);
}
void delete_all_message_show()
{
		check21 = gtk_check_button_new_with_label(_("no1"));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check21), FALSE);
		gtk_label_set_justify((GtkLabel*)check21, GTK_JUSTIFY_LEFT);
		gtk_label_set_line_wrap((GtkLabel*)check21, TRUE);
		gtk_box_pack_start(GTK_BOX(vbox21), check21, FALSE, FALSE, 0);
		i++;

		check22 = gtk_check_button_new_with_label(_("no2"));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check22), FALSE);
		gtk_box_pack_start(GTK_BOX(vbox21), check22, FALSE, FALSE, 0);
		i++;
	
		check23 = gtk_check_button_new_with_label(_("no3"));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check23), FALSE);
		gtk_box_pack_start(GTK_BOX(vbox21), check23, FALSE, FALSE, 0);
		i++;
	
		check24 = gtk_check_button_new_with_label(_("no4"));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check24), FALSE);
		gtk_box_pack_start(GTK_BOX(vbox21), check24, FALSE, FALSE, 0);
		i++;
	
		check25 = gtk_check_button_new_with_label(_("server"));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check25), FALSE);
		gtk_box_pack_start(GTK_BOX(vbox21), check25, FALSE, FALSE, 0);
		i++;
	
}
void delete_all_message_delete()
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check21)))
	{
		strcpy(name, "no1");
		name2ph_num(name);
		delete_m1(login_buff, ph_num);
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check22)))
	{
		strcpy(name, "no2");
		name2ph_num(name);
		delete_m1(login_buff, ph_num);
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check23)))
	{
		strcpy(name, "no3");
		name2ph_num(name);
		delete_m1(login_buff, ph_num);
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check24)))
	{
		strcpy(name, "no4");
		name2ph_num(name);
		delete_m1(login_buff, ph_num);
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check25)))
	{
		strcpy(name, "server");
		name2ph_num(name);
		delete_m1(login_buff, ph_num);
	}
	show_info2();
}
void show1()
{
	//window2
	window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window2), _("删除所有短信"));
	gtk_window_set_default_size(GTK_WINDOW(window2), 430, 550);
	gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_MOUSE);
	gtk_window_set_icon(GTK_WINDOW(window2), create_pixbuf("1.ico"));

	gtk_widget_set_size_request(window2, 430, 550);
	gtk_window_set_resizable(GTK_WINDOW(window2), FALSE);

	fixed2 = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window2), fixed2);
	vbox21 = gtk_vbox_new(TRUE, 1);
	gtk_fixed_put(GTK_FIXED(fixed2), vbox21, 30, 60);

	label21 = gtk_label_new(_("将会删除选中联系人的所有短信记录"));
	gtk_fixed_put(GTK_FIXED(fixed2), label21, 30, 30);

	button21 = gtk_button_new_with_label(_("返回"));
	gtk_fixed_put(GTK_FIXED(fixed2), button21, 80, 500);
	gtk_widget_set_size_request(button21, 80, 35);

	button22 = gtk_button_new_with_label(_("删除"));
	gtk_fixed_put(GTK_FIXED(fixed2), button22, 280, 500);
	gtk_widget_set_size_request(button22, 80, 35);

	/*button23 = gtk_button_new_with_label(_("取消"));
	gtk_fixed_put(GTK_FIXED(fixed2), button23, 330, 500);
	gtk_widget_set_size_request(button23, 80, 35);*/

	i = 0;
	delete_all_message_show();

	g_signal_connect_swapped(G_OBJECT(button21), "clicked",
		G_CALLBACK(des_show), G_OBJECT(window2));

	g_signal_connect(G_OBJECT(button22), "clicked",
		G_CALLBACK(delete_all_message_delete), NULL);

	/*g_signal_connect(G_OBJECT(button23), "clicked",
	G_CALLBACK(show2), NULL);*/

	gtk_widget_show_all(window2);
}
void show2()
{
	//window3
	window3 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window3), _("即时短信"));
	gtk_window_set_default_size(GTK_WINDOW(window3), 430, 550);
	gtk_window_set_position(GTK_WINDOW(window3), GTK_WIN_POS_MOUSE);
	gtk_container_set_border_width(GTK_CONTAINER(window3), 10);
	gtk_window_set_icon(GTK_WINDOW(window3), create_pixbuf("1.ico"));

	gtk_widget_set_size_request(window3, 430, 550);
	gtk_window_set_resizable(GTK_WINDOW(window3), FALSE);

	fixed3 = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window3), fixed3);

	label31 = gtk_label_new(_("1.请选择联系人"));
	gtk_fixed_put(GTK_FIXED(fixed3), label31, 30, 50);

	combo3 = gtk_combo_box_text_new();
	gtk_combo_box_text_append_text((GtkComboBoxText*)combo3, _("no1"));
	gtk_combo_box_text_append_text((GtkComboBoxText*)combo3, _("no2"));
	gtk_combo_box_text_append_text((GtkComboBoxText*)combo3, _("no3"));
	gtk_combo_box_text_append_text((GtkComboBoxText*)combo3, _("no4"));
	gtk_combo_box_text_append_text((GtkComboBoxText*)combo3, _("server"));
	gtk_combo_box_set_active((GtkComboBox*)combo3, FALSE);
	gtk_fixed_put(GTK_FIXED(fixed3), combo3, 150, 50);

	label32 = gtk_label_new(_("2.请输入收信人手机号"));
	gtk_fixed_put(GTK_FIXED(fixed3), label32, 30, 150);

	entry32 = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed3), entry32, 200, 150);

	label33 = gtk_label_new(_("请选择发短信方式"));
	gtk_fixed_put(GTK_FIXED(fixed3), label33, 30, 250);

	radio_button1 = gtk_radio_button_new_with_label(NULL, _("1.选择联系人"));
	radio_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_button1));
	gtk_fixed_put(GTK_FIXED(fixed3), radio_button1, 50, 300);

	radio_button2 = gtk_radio_button_new_with_label(radio_group, (_("2.输入收信人手机号")));
	gtk_fixed_put(GTK_FIXED(fixed3), radio_button2, 50, 340);

	label34 = gtk_label_new(_("短信内容："));
	gtk_fixed_put(GTK_FIXED(fixed3), label34, 10, 380);

	scrolled3 = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy((GtkScrolledWindow *)scrolled3, GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_widget_set_size_request(scrolled3, 330, 150);
	gtk_fixed_put(GTK_FIXED(fixed3), scrolled3, 10, 400);

	view3 = gtk_text_view_new();
	gtk_container_add(GTK_CONTAINER(scrolled3), view3);

	/*button31 = gtk_button_new_with_label(_("添加联系人"));
	c
	gtk_widget_set_size_request(button31, 80, 35);*/

	button32 = gtk_button_new_with_label(_("发送"));
	gtk_fixed_put(GTK_FIXED(fixed3), button32, 350, 430);
	gtk_widget_set_size_request(button32, 80, 35);

	/*button33 = gtk_button_new_with_label(_("存为草稿"));
	gtk_fixed_put(GTK_FIXED(fixed3), button33, 350, 460);
	gtk_widget_set_size_request(button33, 80, 35);*/

	button34 = gtk_button_new_with_label(_("返回"));
	gtk_fixed_put(GTK_FIXED(fixed3), button34, 350, 490);
	gtk_widget_set_size_request(button34, 80, 35);

	/*entry31 = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed3), entry31, 10, 10);
	gtk_widget_set_size_request(entry31, 330, 35);*/


	/*g_signal_connect(G_OBJECT(button31), "clicked",
	G_CALLBACK(show3), G_OBJECT(window));*/

	g_signal_connect_swapped(G_OBJECT(button32), "clicked",
		G_CALLBACK(send_mes), G_OBJECT(window3));

	/*g_signal_connect(G_OBJECT(button33), "clicked",
	G_CALLBACK(show_info2), G_OBJECT(window));*/

	g_signal_connect_swapped(G_OBJECT(button34), "clicked",
		G_CALLBACK(des_show), G_OBJECT(window3));

	gtk_widget_show_all(window3);
}
void show3()
{
	;
}
void show4()
{
	//window5
	window5 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window5), _("定时短信"));
	gtk_window_set_default_size(GTK_WINDOW(window5), 430, 550);
	gtk_window_set_position(GTK_WINDOW(window5), GTK_WIN_POS_MOUSE);
	gtk_window_set_icon(GTK_WINDOW(window5), create_pixbuf("1.ico"));

	gtk_widget_set_size_request(window5, 430, 550);
	gtk_window_set_resizable(GTK_WINDOW(window5), FALSE);

	fixed5 = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window5), fixed5);
	///---------------------------------------------------------------------------------
	label51 = gtk_label_new(_("1.请选择联系人"));
	gtk_fixed_put(GTK_FIXED(fixed5), label51, 30, 20);

	combo5 = gtk_combo_box_text_new();
	gtk_combo_box_text_append_text((GtkComboBoxText*)combo5, _("no1"));
	gtk_combo_box_text_append_text((GtkComboBoxText*)combo5, _("no2"));
	gtk_combo_box_text_append_text((GtkComboBoxText*)combo5, _("no3"));
	gtk_combo_box_text_append_text((GtkComboBoxText*)combo5, _("no4"));
	gtk_combo_box_text_append_text((GtkComboBoxText*)combo5, _("server"));
	gtk_combo_box_set_active((GtkComboBox*)combo5, FALSE);
	gtk_fixed_put(GTK_FIXED(fixed5), combo5, 150, 20);

	label52 = gtk_label_new(_("2.请输入收信人手机号"));
	gtk_fixed_put(GTK_FIXED(fixed5), label52, 30, 70);

	entry52 = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed5), entry52, 200, 70);

	label55 = gtk_label_new(_("请输入定时短信发送的时间(格式：XXXX/XX/XX/ XX:XX)"));
	gtk_fixed_put(GTK_FIXED(fixed5), label55, 30, 110);

	entry53 = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed5), entry53, 30, 140);

	button55 = gtk_button_new_with_label(_("点击获取当前系统时间"));
	gtk_fixed_put(GTK_FIXED(fixed5), button55, 30, 190);
	gtk_widget_set_size_request(button55, 80, 35);

	//标签初始化但不显示，为了get_time函数中可以删除
	label56 = gtk_label_new(_(""));
	gtk_fixed_put(GTK_FIXED(fixed5), label56, 200, 190);
	label57 = gtk_label_new(_(""));
	gtk_fixed_put(GTK_FIXED(fixed5), label57, 200, 220);

	label53 = gtk_label_new(_("请选择发短信方式"));
	gtk_fixed_put(GTK_FIXED(fixed5), label53, 30, 260);

	radio_button51 = gtk_radio_button_new_with_label(NULL, _("1.选择联系人"));
	radio_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_button51));
	gtk_fixed_put(GTK_FIXED(fixed5), radio_button51, 50, 300);

	radio_button52 = gtk_radio_button_new_with_label(radio_group, (_("2.输入收信人手机号")));
	gtk_fixed_put(GTK_FIXED(fixed5), radio_button52, 50, 340);

	label54 = gtk_label_new(_("短信内容："));
	gtk_fixed_put(GTK_FIXED(fixed5), label54, 10, 380);

	scrolled5 = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy((GtkScrolledWindow *)scrolled5, GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_widget_set_size_request(scrolled5, 330, 150);
	gtk_fixed_put(GTK_FIXED(fixed5), scrolled5, 10, 400);

	view5 = gtk_text_view_new();
	gtk_container_add(GTK_CONTAINER(scrolled5), view5);

	button52 = gtk_button_new_with_label(_("发送"));
	gtk_fixed_put(GTK_FIXED(fixed5), button52, 350, 430);
	gtk_widget_set_size_request(button52, 80, 35);

	/*button53 = gtk_button_new_with_label(_("存为草稿"));
	gtk_fixed_put(GTK_FIXED(fixed5), button53, 350, 460);
	gtk_widget_set_size_request(button53, 80, 35);*/

	button54 = gtk_button_new_with_label(_("返回"));
	gtk_fixed_put(GTK_FIXED(fixed5), button54, 350, 490);
	gtk_widget_set_size_request(button54, 80, 35);

	g_signal_connect(G_OBJECT(button52), "clicked",
		G_CALLBACK(time_send), G_OBJECT(window));

	/*g_signal_connect(G_OBJECT(button53), "clicked",
	G_CALLBACK(show_info2), G_OBJECT(window));*/

	g_signal_connect_swapped(G_OBJECT(button54), "clicked",
		G_CALLBACK(des_show), G_OBJECT(window5));

	g_signal_connect(G_OBJECT(button55), "clicked",
		G_CALLBACK(get_time), G_OBJECT(window5));

	//---------------------------------------------------------------------------

	gtk_widget_show_all(window5);
}
void show5()
{
	//window6
	window6 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window6), _("短信"));
	gtk_window_set_default_size(GTK_WINDOW(window6), 430, 550);
	gtk_window_set_position(GTK_WINDOW(window6), GTK_WIN_POS_MOUSE);
	gtk_container_set_border_width(GTK_CONTAINER(window6), 10);
	gtk_window_set_icon(GTK_WINDOW(window6), create_pixbuf("1.ico"));

	gtk_widget_set_size_request(window6, 430, 550);
	gtk_window_set_resizable(GTK_WINDOW(window6), FALSE);

	fixed6 = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window6), fixed6);


	label61 = gtk_label_new(_(name));
	gtk_fixed_put(GTK_FIXED(fixed6), label61, 10, 10);

	name2ph_num(name);//获得手机号
	cout << ph_num << endl;
	select_m(login_buff, ph_num);

	scrolled62 = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy((GtkScrolledWindow *)scrolled62, GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_widget_set_size_request(scrolled62, 380, 330);
	gtk_fixed_put(GTK_FIXED(fixed6), scrolled62, 10, 40);

	vbox61 = gtk_vbox_new(TRUE, 1);
	gtk_scrolled_window_add_with_viewport((GtkScrolledWindow *)(scrolled62), vbox61);
	//gtk_fixed_put(GTK_FIXED(fixed6), vbox61, 10, 40);

	i = 0;
	while (i<row)
	{
		label62 = gtk_label_new(_(msg[i]));
		gtk_label_set_justify((GtkLabel*)label62, GTK_JUSTIFY_LEFT);
		gtk_label_set_line_wrap((GtkLabel*)label62, TRUE);
		gtk_box_pack_start(GTK_BOX(vbox61), label62, FALSE, FALSE, 0);
		i++;
	}

	label64 = gtk_label_new(_("短信内容："));
	gtk_fixed_put(GTK_FIXED(fixed6), label64, 10, 380);

	scrolled61 = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy((GtkScrolledWindow *)scrolled61, GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_widget_set_size_request(scrolled61, 330, 150);
	gtk_fixed_put(GTK_FIXED(fixed6), scrolled61, 10, 400);

	view6 = gtk_text_view_new();
	gtk_container_add(GTK_CONTAINER(scrolled61), view6);

	button62 = gtk_button_new_with_label(_("发送"));
	gtk_fixed_put(GTK_FIXED(fixed6), button62, 350, 410);
	gtk_widget_set_size_request(button62, 80, 35);

	button63 = gtk_button_new_with_label(_("删除"));
	gtk_fixed_put(GTK_FIXED(fixed6), button63, 350, 460);
	gtk_widget_set_size_request(button63, 80, 35);

	button64 = gtk_button_new_with_label(_("返回"));
	gtk_fixed_put(GTK_FIXED(fixed6), button64, 350, 510);
	gtk_widget_set_size_request(button64, 80, 35);

	g_signal_connect(G_OBJECT(button62), "clicked",
		G_CALLBACK(send_mes2), G_OBJECT(window6));

	g_signal_connect_swapped(G_OBJECT(button63), "clicked",
		G_CALLBACK(des_show6), G_OBJECT(window6));

	g_signal_connect_swapped(G_OBJECT(button64), "clicked",
		G_CALLBACK(des_show), G_OBJECT(window6));

	gtk_widget_show_all(window6);
}
void select_delete_message_show()
{
	if (i < row)
	{
		check71 = gtk_check_button_new_with_label(_(msg[i]));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check71), FALSE);
		gtk_label_set_justify((GtkLabel*)check71, GTK_JUSTIFY_LEFT);
		gtk_label_set_line_wrap((GtkLabel*)check71, TRUE);
		gtk_box_pack_start(GTK_BOX(vbox71), check71, FALSE, FALSE, 0);
		i++;
	}
	if (i < row)
	{
		check72 = gtk_check_button_new_with_label(_(msg[i]));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check72), FALSE);
		gtk_box_pack_start(GTK_BOX(vbox71), check72, FALSE, FALSE, 0);
		i++;
	}
	if (i < row)
	{
		check73 = gtk_check_button_new_with_label(_(msg[i]));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check73), FALSE);
		gtk_box_pack_start(GTK_BOX(vbox71), check73, FALSE, FALSE, 0);
		i++;
	}
	if (i < row)
	{
		check74 = gtk_check_button_new_with_label(_(msg[i]));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check74), FALSE);
		gtk_box_pack_start(GTK_BOX(vbox71), check74, FALSE, FALSE, 0);
		i++;
	}
	if (i < row)
	{
		check75 = gtk_check_button_new_with_label(_(msg[i]));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check75), FALSE);
		gtk_box_pack_start(GTK_BOX(vbox71), check75, FALSE, FALSE, 0);
		i++;
	}
	gtk_widget_show_all(window7);
}
void select_delete_message_delete()
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check71)))
	{
		delete_m2(login_buff, ph_num, msg[0 + nxt]);
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check72)))
	{
		delete_m2(login_buff, ph_num, msg[1 + nxt]);
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check73)))
	{
		delete_m2(login_buff, ph_num, msg[2 + nxt]);
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check74)))
	{
		delete_m2(login_buff, ph_num, msg[3 + nxt]);
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check75)))
	{
		delete_m2(login_buff, ph_num, msg[4 + nxt]);
	}
	show_info3();
}
void next()
{
	nxt++;
	gtk_widget_destroy(vbox71);
	vbox71 = gtk_vbox_new(TRUE, 1);
	gtk_fixed_put(GTK_FIXED(fixed7), vbox71, 30, 60);
	select_delete_message_show();
}
void show6()
{
	//window7
	window7 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window7), _("选择删除短信"));
	gtk_window_set_default_size(GTK_WINDOW(window7), 430, 550);
	gtk_window_set_position(GTK_WINDOW(window7), GTK_WIN_POS_MOUSE);
	gtk_window_set_icon(GTK_WINDOW(window7), create_pixbuf("1.ico"));

	gtk_widget_set_size_request(window7, 430, 550);
	gtk_window_set_resizable(GTK_WINDOW(window7), FALSE);

	fixed7 = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window7), fixed7);
	vbox71 = gtk_vbox_new(TRUE, 1);
	gtk_fixed_put(GTK_FIXED(fixed7), vbox71, 30, 60);

	label71 = gtk_label_new(_("将会删除选中的所有短信记录"));
	gtk_fixed_put(GTK_FIXED(fixed7), label71, 30, 30);

	button71 = gtk_button_new_with_label(_("返回"));
	gtk_fixed_put(GTK_FIXED(fixed7), button71, 80, 500);
	gtk_widget_set_size_request(button71, 80, 35);

	button72 = gtk_button_new_with_label(_("删除"));
	gtk_fixed_put(GTK_FIXED(fixed7), button72, 200, 500);
	gtk_widget_set_size_request(button72, 80, 35);

	button73 = gtk_button_new_with_label(_("下一批"));
	gtk_fixed_put(GTK_FIXED(fixed7), button73, 320, 500);
	gtk_widget_set_size_request(button73, 80, 35);

	i = 0;
	select_delete_message_show();
	//每个n个字符，设置一个换行符\n，以免短信内容太长而超出界面
	/*check71 = gtk_check_button_new_with_label(_("hello world, too!你好啊，我觉得这个世界还是很美好的，啦啦啦啦啦，万万没想到啊hello world, too!你好啊，我觉得这个世界还是很美好的，啦啦啦啦啦，万万没想到啊"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check71), FALSE);
	gtk_fixed_put(GTK_FIXED(fixed7), check71, 50, 70);

	check72 = gtk_check_button_new_with_label(_("hello world, too!你好啊，我觉得这个世界还是很美好的，啦啦啦啦啦，万万没想到啊"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check72), FALSE);
	gtk_fixed_put(GTK_FIXED(fixed7), check72, 50, 120);

	check73 = gtk_check_button_new_with_label(_("hello world, too!你好啊，我觉得这个世界还是很美好的，啦啦啦啦啦，万万没想到啊"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check73), FALSE);
	gtk_fixed_put(GTK_FIXED(fixed7), check73, 50, 170);

	check74 = gtk_check_button_new_with_label(_("hello world, too!你好啊，我觉得这个世界还是很美好的，啦啦啦啦啦，万万没想到啊"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check74), FALSE);
	gtk_fixed_put(GTK_FIXED(fixed7), check74, 50, 220);

	check75 = gtk_check_button_new_with_label(_("hello world, too!你好啊，我觉得这个世界还是很美好的，啦啦啦啦啦，万万没想到啊"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check75), FALSE);
	gtk_fixed_put(GTK_FIXED(fixed7), check75, 50, 270);*/

	g_signal_connect_swapped(G_OBJECT(button71), "clicked",
		G_CALLBACK(des_show5), G_OBJECT(window7));

	g_signal_connect(G_OBJECT(button72), "clicked",
		G_CALLBACK(select_delete_message_delete), NULL);

	g_signal_connect(G_OBJECT(button73), "clicked",
		G_CALLBACK(next), NULL);

	gtk_widget_show_all(window7);

}


BOOL SendMes(SOCKET servSocket, char*toname, char*sendname, char*mess)
{
	SOCKET servSock = servSocket;
	memset(sendBuff, 0, sizeof(sendBuff)); 
	if (isLogin)
	{
		time_t t = time(0);
		strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
		strcpy(short_tmp, tmp);
		short_tmp[16] = '\0';

		char toName[32] = { 0 };
		strcpy(toName, toname);
		memcpy(sendBuff, &MesNormal, 2);
		memcpy(sendBuff + 2, toName, 32);
		char myName[1000] = { 0 };
		strcpy(myName, sendname);
		char Message[1000] = { 0 };
		strcpy(Message, mess);
		strcat(myName, ":\n");
		strcat(myName, tmp);
		strcat(myName, "\n");
		strcat(myName, Message);
		memcpy(sendBuff + 34, myName, strlen(myName) + 1);
		sent_insert(login_buff, to_ph_num, sendBuff + 34, short_tmp);
		send(servSock, sendBuff, sizeof(sendBuff)+1, 0);
		cout << sendBuff << endl;
	}
	return TRUE;
}
BOOL SendMes1(SOCKET servSocket, char*toname, char*sendname, char*mess)
{
	SOCKET servSock = servSocket;
	memset(sendBuff, 0, sizeof(sendBuff));
	if (isLogin)
	{
		time_t t = time(0);
		strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
		strcpy(short_tmp, tmp);
		short_tmp[16] = '\0';

		char toName[32] = { 0 };
		strcpy(toName, toname);
		memcpy(sendBuff, &MesTime, 2);
		memcpy(sendBuff + 2, toName, 32);
		char myName[1000] = { 0 };
		strcpy(myName, sendname);
		char Message[1000] = { 0 };
		strcpy(Message, mess);
		strcat(myName, ":\n");
		strcat(myName, tmp);
		strcat(myName, "\n");
		strcat(myName, Message);
		memcpy(sendBuff + 34, myName, strlen(myName) + 1);
		sent_insert(login_buff, to_ph_num, sendBuff + 34, short_tmp);
		send(servSock, sendBuff, sizeof(sendBuff)+1, 0);
		cout << sendBuff << endl;
	}
	return TRUE;
}
BOOL Login(SOCKET logSocket, char*loginname)
{
	SOCKET logsock = logSocket;
	char userName[32];
	strcpy(userName, loginname);
	char loginInfo[64] = { 0 };
	memcpy(loginInfo, &MesLogin, 2);
	memcpy(loginInfo + 2, userName, strlen(userName) + 1);
	int a = send(logsock, loginInfo, strlen(loginInfo), 0);
	return TRUE;
}
BOOL Logout(SOCKET logSocket, char*logoutname)
{
	SOCKET logsock = logSocket;
	char userName[32];
	strcpy(userName, logoutname);
	char logoutInfo[64] = { 0 };
	memcpy(logoutInfo, &MesLogout, 2);
	memcpy(logoutInfo + 2, userName, strlen(userName) + 1);
	int a = send(logsock, logoutInfo, strlen(logoutInfo), 0);
	return TRUE;
}
DWORD WINAPI ProcThread(LPVOID lpParameter)
{
	
	SOCKET cliSocket = *(SOCKET*)lpParameter;
	memset(recvBuf, 0, sizeof(recvBuf));
	while (true)
	{
		memset(recvBuf, 0x00, 1024);
		recvLen = recv(cliSocket, recvBuf, 1024, 0);
		if (recvLen > 0)
		{
			time_t t = time(0);
			strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
			strcpy(short_tmp, tmp);
			short_tmp[16] = '\0';

			megId = *(short*)recvBuf;
			switch (megId)
			{
			case LoginSuc:
			{
							 isLogin = true;
			}
				break;
			case MesNormal:
			{
							  char Mess[1000];
							  memcpy(Mess, recvBuf + 2, sizeof(recvBuf)-2);
							  strcpy(rece_mess, Mess);
							  cout << Mess << endl;
							  i = 0;
							  for (; i < 11; i++)
							  {
								  ph_num[i] = rece_mess[i];
							  }
							  ph_num[i] = '\0';
							  receive_insert(login_buff, ph_num, rece_mess, short_tmp);
							  /*gdk_threads_enter();
							  unread_mess();
							  gdk_threads_leave();*/
			}
				break;
			default:
				break;
			}
		}//end if

	}// end while

	return 0;
}
/*DWORD WINAPI ProcThread1(LPVOID lpParameter)
{
	SOCKET cliSocket = *(SOCKET*)lpParameter;
	locktime = (char*)gtk_entry_get_text((GtkEntry*)entry53);

	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
	strcpy(short_tmp, tmp);
	short_tmp[16] = '\0';
	while (TRUE)
	{
		if (strcmp(locktime, short_tmp) == 0)
		{
			send_mes1();
			CloseHandle(ProcThread1);
		}
		//_sleep(30*1000);
		strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
		strcpy(short_tmp, tmp);
		short_tmp[16] = '\0';
	}
}*/
void login()
{
	login_buff = (char*)gtk_entry_get_text((GtkEntry*)entry);
	Login(clientSock, login_buff);                                 //NO.1
	gtk_widget_hide(window);
	show();
}
void send_mes()//即时短信发送
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button1)))
	{
		to_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo3));
		name2ph_num(to_name);
		to_ph_num = ph_num;
	}
	else
	{
		to_ph_num = (char*)gtk_entry_get_text((GtkEntry*)entry32);
	}


	GtkTextBuffer* Send_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view3));
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(Send_buffer), &start, &end);
	text_mess = (char*)gtk_text_buffer_get_text((GtkTextBuffer*)Send_buffer, &(const GtkTextIter)start, &(const GtkTextIter)end, NULL);

	s_len = strlen(text_mess);
	i = j = circ = 0;
	//获取系统时间
	time_t t = time(0);
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
	strcpy(short_tmp, tmp);
	short_tmp[16] = '\0';

	if (strlen(text_mess) <= 70)
	{
		SendMes(clientSock, to_ph_num, login_buff, text_mess);
		show_info1();
	}
	else
	{
		while (i < s_len)
		{
			j = i - 70 * circ;
			while (j < 70)
			{
				t_mess[j] = text_mess[i];
				i++;
				j++;
			}
			t_mess[j] = '\0';
			circ++;
			SendMes(clientSock, to_ph_num, login_buff, t_mess);
			_sleep(1100);//延时，以保证不会只发第一条短信
		}
		show_info1();
	}
}
void send_mes1()
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_button51)))
	{
		to_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo5));
		name2ph_num(to_name);
		to_ph_num = ph_num;
	}
	else
	{
		to_ph_num = (char*)gtk_entry_get_text((GtkEntry*)entry52);
	}

	GtkTextBuffer* Send_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view5));
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(Send_buffer), &start, &end);
	text_mess = (char*)gtk_text_buffer_get_text((GtkTextBuffer*)Send_buffer, &(const GtkTextIter)start, &(const GtkTextIter)end, NULL);

	s_len = strlen(text_mess);
	i = j = circ = 0;

	//获取系统时间
	time_t t = time(0);
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
	strcpy(short_tmp, tmp);
	short_tmp[16] = '\0';

	if (strlen(text_mess) <= 70)
	{
		SendMes1(clientSock, to_ph_num, login_buff, text_mess);
		show_info1();
	}
	else
	{
		while (i < s_len)
		{
			j = i - 70 * circ;
			while (j < 70)
			{
				t_mess[j] = text_mess[i];
				i++;
				j++;
			}
			t_mess[j] = '\0';
			circ++;
			SendMes1(clientSock, to_ph_num, login_buff, t_mess);
			_sleep(1100);//延时，以保证不会只发第一条短信
		}
		show_info1();
	}
}
void send_mes2()
{
	to_ph_num = ph_num;

	GtkTextBuffer* Send_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view6));
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(Send_buffer), &start, &end);
	text_mess = (char*)gtk_text_buffer_get_text((GtkTextBuffer*)Send_buffer, &(const GtkTextIter)start, &(const GtkTextIter)end, NULL);

	s_len = strlen(text_mess);
	i = j = circ = 0;

	//获取系统时间
	time_t t = time(0);
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
	strcpy(short_tmp, tmp);
	short_tmp[16] = '\0';

	if (strlen(text_mess) <= 70)
	{
		SendMes(clientSock, ph_num, login_buff, text_mess);
		show_info1();
	}
	else
	{
		while (i < s_len)
		{
			j = i - 70 * circ;
			while (j < 70)
			{
				t_mess[j] = text_mess[i];
				i++;
				j++;
			}
			t_mess[j] = '\0';
			circ++;
			SendMes(clientSock, ph_num, login_buff, t_mess);
			_sleep(1100);//延时，以保证不会只发第一条短信
		}
		show_info1();
	}

}
void time_send()
{
	strcpy(locktime, (char*)gtk_entry_get_text((GtkEntry*)entry53));
	send_mes1();
}
int main(int argc, char *argv[])
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


	clientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSock == -1)
	{
		perror("socket创建出错！");
		exit(1);
	}

	cout << "请输入服务器ip：" << endl;
	cin >> server_ip;
	cout << "请输入使用的端口号：" << endl;
	cin >> using_port;

	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = inet_addr(server_ip);
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(using_port);


	int ret = connect(clientSock, (sockaddr*)&addrClient, sizeof(SOCKADDR));
	HANDLE thHandle = CreateThread(NULL, 0, ProcThread, &clientSock, 0, NULL);
	/*if (ret)
	{
	printf("连接失败\n");
	//return -1;
	}
	
	if (NULL == thHandle)
	{
	cout << "Create Thread failed !" << endl;
	//return -1;
	}*/

	opendb();

	gtk_init(&argc, &argv);
	//window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), _("短信收发系统客户端-登陆"));
	gtk_window_set_default_size(GTK_WINDOW(window), 430, 550);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);
	gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("1.ico")); 

	gtk_widget_set_size_request(window, 430, 550);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	fixed0 = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), fixed0);

	/*image = gtk_image_new_from_file("background.jpg");
	gtk_container_add(GTK_CONTAINER(fixed0), image);*/
	
	label = gtk_label_new(_("请输入你的电话号码"));
	gtk_fixed_put(GTK_FIXED(fixed0), label, 160, 160);

	entry = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed0), entry, 140, 190);

	button01 = gtk_button_new_with_label(_("登陆"));
	gtk_fixed_put(GTK_FIXED(fixed0), button01, 180, 250);
	gtk_widget_set_size_request(button01, 80, 35);

	button02 = gtk_button_new_with_label(_("关闭程序"));
	gtk_fixed_put(GTK_FIXED(fixed0), button02, 180, 300);
	gtk_widget_set_size_request(button02, 80, 35);

	g_signal_connect(G_OBJECT(button01), "clicked",
		G_CALLBACK(login), G_OBJECT(window));

	g_signal_connect(G_OBJECT(button02), "clicked",
		G_CALLBACK(gtk_main_quit), G_OBJECT(window));

	//destroy
	g_signal_connect_swapped(G_OBJECT(window), "destroy",
		G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(window);

	//gdk_threads_enter();
	gtk_main();
	//gdk_threads_leave();
	closesocket(clientSock);
	CloseHandle(thHandle);
	WSACleanup();
	sqlite3_close(db);
	return 0;
}