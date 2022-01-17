#ifndef __FUNC_H__
#define __FUNC_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>
#define IP "192.168.1.8"
#define PORT 8888
#define ERR_MSG(msg) do{\
	printf("%d:%s",__LINE__,__func__);\
	perror(msg);\
}while (0);

struct info{
	char name[20];
	int ipone_num;
	char pwd[20];
	char address[50];
	float salary;
	int age;
	char type;
	int flag;
	int Mflag;
	int newipone;
}__attribute__((packed));

struct Db_And_fd{
	sqlite3 *db;
	int newfd;
}__attribute__((packed));
//服务端功能函数
void *Ser_Cli_Comu(void *arg);
int inet_init();
sqlite3 *sqlite_init();
int Ser_Login(struct info msg,sqlite3 *db,int newfd);
int Ser_ADD(struct info msg,sqlite3 *db,int newfd);
int Ser_Register(struct info msg,sqlite3 *db,int newfd);
int Ser_Del(struct info msg,sqlite3 *db,int newfd);
int Ser_Search(struct info msg,sqlite3 *db,int newfd);
int Ser_Modify(struct info msg,sqlite3 *db,int newfd);

//客户端
int cli_inet_init();
int Login_System(int sfd);
int Do_Login(int sfd);
int Administrator_Login(int sfd);
int Administrator_Op();
int Employee_Login(int sfd);
int Employee_Op();
int Add_Employee(int sfd);
int Do_Register(int sfd);
int Administrator_Register(int sfd);
int Employee_Register(int sfd);
int Del_Employee(int sfd);
int Modify_Employee(int sfd,struct info msg);
int Search_EMployee(int sfd,struct info msg);

#endif
