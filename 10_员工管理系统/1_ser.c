#include "func.h"
int main(int argc, const char *argv[])
{
	//网络初始化
	int sfd,newfd;
	pthread_t tid;
	sqlite3 *db = NULL;
	struct Db_And_fd dbfd;
	sfd = inet_init();
	//数据库初始化
	db = sqlite_init();
	dbfd.db = db;
	while(1)
	{
		newfd = accept(sfd,NULL,NULL);
		if(newfd < 0){
			ERR_MSG("accept");
			return -1;
		}
		dbfd.newfd = newfd;
		printf("客户端连接成功,newfd = %d\n",newfd);
		if(pthread_create(&tid,NULL,Ser_Cli_Comu,&dbfd)<0){
			ERR_MSG("pthread_create");
			return -1;
		}
	}
	pthread_join(tid,NULL);
	close(sfd);
	return 0;
}

void *Ser_Cli_Comu(void *arg)
{
	int newfd,ret;
	sqlite3 *db;
	struct Db_And_fd dbfd = *(struct Db_And_fd*)arg;
	pthread_detach(pthread_self());
	newfd =dbfd.newfd;
	db = dbfd.db;
	struct info msg;
	while(1)
	{
		ret = recv(newfd,&msg,sizeof(msg),0);
		if(ret < 0)
		{
			ERR_MSG("recv");
			return NULL;
		}else if(ret == 0)
		{
			printf("客户端关闭..........\n");
			close(newfd);
			return NULL;	
		}
		switch(msg.type)
		{
		case 'L':
			//登录
			Ser_Login(msg,db,newfd);
			break;
		case 'R':
			//注册
			Ser_Register(msg,db,newfd);
			break;
		case 'I':
			//增加
			Ser_ADD(msg,db,newfd);
			break;
		case 'D':
			//删除
			Ser_Del(msg,db,newfd);
			break;
		case 'C':
			//修改
			Ser_Modify(msg,db,newfd);
			break;
		case 'F':
			//查找
			Ser_Search(msg,db,newfd);
			break;
		default:
			break;
		}
	}
	pthread_exit(NULL);
}


