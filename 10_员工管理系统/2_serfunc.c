#include "func.h"
int inet_init()
{
	int sfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0){
		ERR_MSG("socket");
		return -1;
	}
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.s_addr = inet_addr(IP);
	socklen_t addrlen = sizeof(sin);
	if(bind(sfd,(struct sockaddr *)&sin,addrlen) < 0){
		ERR_MSG("bind");
		return -1;
	}
	printf("绑定成功\n");
	if(listen(sfd,10)<0){
		ERR_MSG("listen");
		return -1;
	}
	printf("监听成功\n");
	return sfd;
}
sqlite3 *sqlite_init()
{
	sqlite3 *db = NULL;
	char sql[128] = "create table if not exists stu_info(name char,ipone int primary key,address char,salary float,age int);";
	char *errmsg;
	if(sqlite3_open("./stu.db",&db) != SQLITE_OK)
	{
		printf("sqlite3_open failed:%s\n",sqlite3_errmsg(db));
		return NULL;
	}
	//创建员工信息表
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
	{
		printf("create stu_info failed:%s:%d\n",errmsg,__LINE__);
		return NULL;
	}
	printf("员工信息表创建成功\n");
	bzero(sql,sizeof(sql));
	strcpy(sql,"create table if not exists pwd_info(ipone int,pwd char,flag int);");
	//创建账号密码表
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
	{
		printf("create pwd_info failed:%s:%d\n",errmsg,__LINE__);
		return NULL;
	}
	printf("账号密码表创建成功\n");
	return db;
}

int Ser_Login(struct info msg,sqlite3 *db,int newfd)
{
	char sql[128] = "";
	char **result = NULL;
	int row,column;
	char *errmsg;
	sprintf(sql,"select * from pwd_info where ipone=%d and (pwd=%s and flag=%d);",msg.ipone_num,msg.pwd,msg.flag);
	if(sqlite3_get_table(db,sql,&result,&row,&column,&errmsg) != SQLITE_OK)
	{
		printf("%d:%s\n",__LINE__,errmsg);
		return -1;
	}
	bzero(sql,sizeof(sql));
	if(row == 0)
	{
		strcpy(sql,"notfound");
		if(send(newfd,sql,sizeof(sql),0) < 0)
		{
			ERR_MSG("send");
			return -1;
		}

	}else
	{
		strcpy(sql,"success");
		if(send(newfd,sql,sizeof(sql),0) < 0)
		{
			ERR_MSG("send");
			return -1;
		}

	}
	return 0;
}
//注册
int Ser_Register(struct info msg,sqlite3 *db,int newfd)
{
	char sql[250] = "";
	char **result = NULL;
	int row,column;
	char *errmsg;
	sprintf(sql,"select * from pwd_info where ipone=%d;",msg.ipone_num);
	if(sqlite3_get_table(db,sql,&result,&row,&column,&errmsg) != SQLITE_OK)
	{
		printf("%d:%s\n",__LINE__,errmsg);
		return -1;
	}
	bzero(sql,sizeof(sql));
	if(row == 0)
	{
		sprintf(sql,"insert into pwd_info values(%d,%s,%d);",msg.ipone_num,msg.pwd,msg.flag);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
		{
			printf("%d:%s\n",__LINE__,errmsg);
			return -1;
		}
		bzero(sql,sizeof(sql));
		strcpy(sql,"T");
		if(send(newfd,sql,1,0)<0)
		{
			ERR_MSG("send");
			return -1;
		}
	}else
	{
		strcpy(sql,"F");
		if(send(newfd,sql,1,0) < 0)
		{
			ERR_MSG("send");
			return -1;
		}
	}
	return 0;

}
//增
int Ser_ADD(struct info msg,sqlite3 *db,int newfd)
{
	char sql[500] = "";
	char **result = NULL;
	int row,column;
	char *errmsg;
	sprintf(sql,"select * from stu_info where ipone=%d;",msg.ipone_num);
	if(sqlite3_get_table(db,sql,&result,&row,&column,&errmsg) != SQLITE_OK)
	{
		printf("%d:%s\n",__LINE__,errmsg);
		return -1;
	}
	bzero(sql,sizeof(sql));
	if(row == 0)
	{
		sprintf(sql,"insert into stu_info values(\"%s\",%d,\"%s\",%f,%d);",msg.name,msg.ipone_num,msg.address,msg.salary,msg.age);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
		{
			printf("%d:%s\n",__LINE__,errmsg);
			return -1;
		}
		bzero(sql,sizeof(sql));
		strcpy(sql,"T");
		if(send(newfd,sql,1,0)<0)
		{
			ERR_MSG("send");
			return -1;
		}
	}else
	{
		strcpy(sql,"F");
		if(send(newfd,sql,1,0) < 0)
		{
			ERR_MSG("send");
			return -1;
		}
	}
	return 0;
}
int Ser_Del(struct info msg,sqlite3 *db,int newfd)
{
	//删除
	char sql[250] = "";
	char **result = NULL;
	int row,column;
	char *errmsg;
	sprintf(sql,"select * from stu_info where ipone=%d and name=\"%s\";",msg.ipone_num,msg.name);
	if(sqlite3_get_table(db,sql,&result,&row,&column,&errmsg) != SQLITE_OK)
	{
		printf("%d:%s\n",__LINE__,errmsg);
		return -1;
	}
	bzero(sql,sizeof(sql));
	if(row != 0)
	{
		char choose;
		strcpy(sql,"A");
		if(send(newfd,sql,1,0) <0 )
		{
			ERR_MSG("send");
			return -1;
		}
		if(recv(newfd,&choose,sizeof(choose),0)<0)
		{
			ERR_MSG("recv");
			return -1;
		}
		if(choose == 'y'||choose == 'Y')
		{
			sprintf(sql,"delete from stu_info where ipone=%d and name=\"%s\";",msg.ipone_num,msg.name);
			if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
			{
				printf("%d:%s\n",__LINE__,errmsg);
				return -1;
			}
			bzero(sql,sizeof(sql));
			strcpy(sql,"T");
			if(send(newfd,sql,1,0)<0)
			{
				ERR_MSG("send");
				return -1;
			}
		}else{
			strcpy(sql,"F");
			if(send(newfd,sql,1,0) < 0)
			{
				ERR_MSG("send");
				return -1;
			}
		}

	}else{
		strcpy(sql,"B");
		if(send(newfd,sql,1,0) < 0)
		{
			ERR_MSG("send");
			return -1;
		}
	}
	return 0;
}

int Ser_Modify(struct info msg,sqlite3 *db,int newfd)
{
	//修改
	char sql[200] = "";
	char *errmsg;
	switch(msg.Mflag)
	{
	case 1:
	//	printf("%d\n",msg.ipone_num);
		sprintf(sql,"update stu_info set name=\"%s\" where ipone=%d;",msg.name,msg.ipone_num);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
		{
			printf("%d:%s\n",__LINE__,errmsg);
			return -1;
		}
		bzero(sql,sizeof(sql));
		strcpy(sql,"T");
		if(send(newfd,sql,1,0)<0)
		{
			ERR_MSG("send");
			return -1;
		}

		break;
	case 2:
		sprintf(sql,"update stu_info set ipone=%d where ipone=%d or name=\"%s\";",msg.newipone,msg.ipone_num,msg.name);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
		{
			printf("%d:%s\n",__LINE__,errmsg);
			return -1;
		}
		bzero(sql,sizeof(sql));
		strcpy(sql,"T");
		if(send(newfd,sql,1,0)<0)
		{
			ERR_MSG("send");
			return -1;
		}

		break;
	case 3:
		sprintf(sql,"update stu_info set address=\"%s\" where ipone=%d;",msg.address,msg.ipone_num);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
		{
			printf("%d:%s\n",__LINE__,errmsg);
			return -1;
		}
		bzero(sql,sizeof(sql));
		strcpy(sql,"T");
		if(send(newfd,sql,1,0)<0)
		{
			ERR_MSG("send");
			return -1;
		}

		break;
	case 4:
		sprintf(sql,"update stu_info set salary=%f where ipone=%d;",msg.salary,msg.ipone_num);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
		{
			printf("%d:%s\n",__LINE__,errmsg);
			return -1;
		}
		bzero(sql,sizeof(sql));
		strcpy(sql,"T");
		if(send(newfd,sql,1,0)<0)
		{
			ERR_MSG("send");
			return -1;
		}

		break;
	case 5:
		sprintf(sql,"update stu_info set age=%d where ipone=%d;",msg.age,msg.ipone_num);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg)!=SQLITE_OK)
		{
			printf("%d:%s\n",__LINE__,errmsg);
			return -1;
		}
		bzero(sql,sizeof(sql));
		strcpy(sql,"T");
		if(send(newfd,sql,1,0)<0)
		{
			ERR_MSG("send");
			return -1;
		}
		break;
	default:
		break;
	}
	return 0;
}
int Ser_Search(struct info msg,sqlite3 *db,int newfd)
{
	//查找
	char sql[128] = "";
	char **result = NULL;
	int row,column;
	char *errmsg;
	sprintf(sql,"select * from stu_info where ipone=%d;",msg.ipone_num);
	if(sqlite3_get_table(db,sql,&result,&row,&column,&errmsg)!=SQLITE_OK)
	{
		printf("%d:%s\n",__LINE__,errmsg);
		return -1;
	}
	bzero(sql,sizeof(sql));
	if(row != 0)
	{   
		strcpy(sql,"T");
		if(send(newfd,sql,1,0) <0)
		{
			ERR_MSG("send");
			return -1;
		}
		bzero(sql,sizeof(sql));
		sprintf(sql,"%-10s%-10s%-10s%-10s%-10s",result[0],result[1],result[2],result[3],result[4]);
		if(send(newfd,sql,sizeof(sql),0) < 0)
		{
			ERR_MSG("send");
			return -1;
		}
		bzero(sql,sizeof(sql));
		sprintf(sql,"%-10s%-10s%-10s%-10s%-10s",result[5],result[6],result[7],result[8],result[9]);
		if(send(newfd,sql,sizeof(sql),0) < 0)
		{
			ERR_MSG("send");
			return -1;
		}
	}else{
		strcpy(sql,"F");
		if(send(newfd,sql,1,0) <0)
		{
			ERR_MSG("send");
			return -1;
		}
	}
	return 0;
}
