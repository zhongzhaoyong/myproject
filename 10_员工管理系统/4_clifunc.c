#include "func.h"
int cli_inet_init()
{
	int sfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0)
	{
		ERR_MSG("socket");
		return -1;
	}
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.s_addr = inet_addr(IP);
	socklen_t addrlen = sizeof(sin);
	if(connect(sfd,(struct sockaddr *)&sin,addrlen) < 0)
	{
		ERR_MSG("connect");
		return -1;
	}
	return sfd;
}

int Login_System(int sfd)
{
	
	char choose;
	while(1)
	{
		
		printf("**************************************\n");
		printf("*************员工管理系统*************\n");
		printf("****************1.登录****************\n");
		printf("****************2.注册****************\n");
		printf("****************3.退出****************\n");
		printf("**************************************\n");
		printf("请输入选项>>>");
		scanf("%c",&choose);
		while(getchar()!=10);
		switch(choose)
		{
		case '1':
			//登录
			Do_Login(sfd);
			break;
		case '2':
			//注册
			Do_Register(sfd);
			break;
		case '3':
			exit(0);
			break;
		default:
			printf("输入错误\n");
			continue;
		}
		
	}
	return 0;
}

int Do_Register(int sfd)
{
	//注册
	char choose;
	int flag = 0;
	while(1)
	{	

		printf("***************************************\n");
		printf("***********1.管理员注册****************\n");
		printf("***********2.员工注册******************\n");
		printf("***********3.退出**********************\n");
		printf("***************************************\n");
		printf("请选择你的注册方式>>>");
		scanf("%c",&choose);
		while(getchar()!=10);
		switch(choose)
		{
		case '1':
			//管理员注册
			Administrator_Register(sfd);
			break;
		case '2':
			//员工注册
			Employee_Register(sfd);
			break;
		case '3':
			//退出
			flag = 1;
			break;
		default:
			printf("输入错误\n");
			continue;
		}
		if(flag == 1)
			break;
		}
	return 0;
}
int Administrator_Register(int sfd)
{   //管理员注册
	int pwd = 111;
	int upwd,ipone;
	char apwd[20] = "";
	struct info msg;
	printf("请输入注册密码>>>");
	scanf("%d",&upwd);
	while(getchar()!=10);
	if(upwd != pwd)
	{
		printf("密码输入错误\n");
		return -1;
	}else{
		printf("请输入注册手机号>>>");
		scanf("%d",&ipone);
		while(getchar()!=10);
		printf("请输入密码>>>");
		fgets(apwd,sizeof(apwd),stdin);
		apwd[strlen(apwd)-1] = 0;
		msg.ipone_num = ipone;
		strcpy(msg.pwd,apwd);
		msg.flag = 1;
		msg.type = 'R';
		if(send(sfd,&msg,sizeof(msg),0) < 0)
		{
			ERR_MSG("send");
			return -1;
		}	
		bzero(apwd,sizeof(apwd));
		if(recv(sfd,&apwd,sizeof(apwd),0) < 0)
		{
			ERR_MSG("recv");
			return -1;
		}
		if(strcmp(apwd,"T")==0)
		{
			printf("注册成功\n");
			return 0;
		}else if(strcmp(apwd,"F") == 0)
		{
			printf("注册失败\n");
			return -1;
		}
	}
	return 0;
}
int Employee_Register(int sfd)
{   //员工注册
	int ipone;
	char apwd[128] = "";
	struct info msg;
	printf("请输入注册手机号>>>");
	scanf("%d",&ipone);
	while(getchar()!=10);
	printf("请输入密码>>>");
	fgets(apwd,sizeof(apwd),stdin);
	apwd[strlen(apwd)-1] = 0;
	msg.ipone_num = ipone;
	strcpy(msg.pwd,apwd);
	msg.flag = 0;
	msg.type = 'R';
	if(send(sfd,&msg,sizeof(msg),0) < 0)
	{
		ERR_MSG("send");
		return -1;
	}	
	bzero(apwd,sizeof(apwd));
	if(recv(sfd,&apwd,sizeof(apwd),0) < 0)
	{
		ERR_MSG("recv");
		return -1;
	}
	if(strcmp(apwd,"T")==0)
	{
		printf("注册成功\n");
		return 0;
	}else if(strcmp(apwd,"F") == 0)
	{
		printf("注册失败\n");
		return -1;
	}
	return 0;
}
int Do_Login(int sfd)
{   //登录
	char choose;
	int flag = 0;
	while(1)
	{		
		printf("***************************************\n");
		printf("***********1.管理员登录****************\n");
		printf("***********2.员工登录******************\n");
		printf("***********3.退出**********************\n");
		printf("***************************************\n");
		printf("请选择你的登录方式>>>");
		scanf("%c",&choose);
		while(getchar()!=10);
		switch(choose)
		{
		case '1':
			//管理员登录
			Administrator_Login(sfd);
			break;
		case '2':
			//员工登录
			Employee_Login(sfd);
			break;
		case '3':
			flag = 1;
			break;
		default:
			printf("输入错误\n");
			break;
		}
		if(flag == 1)
			break;
		
	}
	return 0;
}
int Administrator_Login(int sfd)
{
	//管理员登录
	printf("请输入管理员手机号\n");
	int number;
	char pwd[128] = {0};
	struct info msg;
	scanf("%d",&number);
	while(getchar()!=10);
	printf("请输入管理员密码\n");
	fgets(pwd,sizeof(pwd),stdin);
	pwd[strlen(pwd)-1] = 0;
	msg.ipone_num = number;
	memcpy(msg.pwd,pwd,sizeof(pwd));
	msg.flag = 1;
	msg.type = 'L';
	if(send(sfd,&msg,sizeof(msg),0) < 0)
	{
		ERR_MSG("send");
		return -1;
	}
	bzero(pwd,sizeof(pwd));
	if(recv(sfd,&pwd,sizeof(pwd),0) < 0)
	{
		ERR_MSG("recv");
		return -1;
	}
	if(strcmp(pwd,"notfound") == 0)
	{
		printf("账号密码错误\n");
		return -1;
	}else if(strcmp(pwd,"success") == 0)
	{
		printf("登录成功\n");
		Administrator_Op(sfd,msg);
	}
	return 0;
}

int Employee_Login(int sfd)
{
	//员工登录
	printf("请输入手机号\n");
	int number;
	char pwd[128] = {0};
	struct info msg;
	scanf("%d",&number);
	while(getchar()!=10);
	printf("请输入密码\n");
	fgets(pwd,sizeof(pwd),stdin);
	pwd[strlen(pwd)-1] = 0;
	msg.ipone_num = number;
	memcpy(msg.pwd,pwd,sizeof(pwd));
	msg.flag = 0;
	msg.type = 'L';
	if(send(sfd,&msg,sizeof(msg),0) < 0)
	{
		ERR_MSG("send");
		return -1;
	}
	bzero(pwd,sizeof(pwd));
	if(recv(sfd,&pwd,sizeof(pwd),0) < 0)
	{
		ERR_MSG("recv");
		return -1;
	}
	if(strcmp(pwd,"notfound") == 0)
	{
		printf("账号密码错误\n");
		return -1;
	}else if(strcmp(pwd,"success") == 0)
	{
		printf("登录成功\n");
		Employee_Op(sfd,msg);
	}
	return 0;

}
int Administrator_Op(int sfd,struct info msg)
{   //管理员操作
	char choose;
	int flag = 0;
	while(1)
	{

		printf("********************************************\n");
		printf("****************1.增加员工******************\n");
		printf("****************2.删除员工******************\n");
		printf("****************3.修改员工信息**************\n");
		printf("****************4.查找员工信息**************\n");
		printf("****************5.退出**********************\n");
		printf("********************************************\n");
		printf("请输入你的选项>>>");
		scanf("%c",&choose);
		while(getchar()!=10);
		switch(choose)
		{
		case '1':
			//增加员工
			Add_Employee(sfd);
			break;
		case '2':
			//删除员工
			Del_Employee(sfd);
			break;
		case '3':
			//修改员工信息
			Modify_Employee(sfd,msg);
			break;
		case '4':
			//查找员工
			Search_EMployee(sfd,msg);
			break;
		case '5':
			flag = 1;
			break;
		default:
			printf("输入错误\n");
			break;
		}
		if(flag == 1)
			break;
		
	}
	return 0;
}
int Employee_Op(int sfd,struct info msg)
{	//员工操作
	char choose;
	int flag = 0;
	while(1)
	{

		printf("********************************************\n");
		printf("****************1.修改信息******************\n");
		printf("****************2.查看信息******************\n");
		printf("****************3.退出**********************\n");
		printf("********************************************\n");
		printf("请输入你的选项>>>");
		scanf("%c",&choose);
		while(getchar()!=10);
		switch(choose)
		{
		case '1':
			//修改信息
			Modify_Employee(sfd,msg);
			break;
		case '2':
			//查看信息
			Search_EMployee(sfd,msg);
			break;
		case '3':
			//退出
			flag = 1;
			break;
		default:
			printf("输入错误\n");
			break;
		}
		if(flag == 1)
		{
			printf("退出成功\n");
			break;
		}
	

	}
	return 0;

}

//增
int Add_Employee(int sfd)
{
	struct info msg;
	char buf[128] = "";
	printf("请输入员工的姓名>>>");
	fgets(msg.name,sizeof(msg.name),stdin);
	msg.name[strlen(msg.name)-1] = 0;
	printf("请输入员工的手机号>>>");
	scanf("%d",&(msg.ipone_num));
	while(getchar()!=10);
	printf("请输入员工的住址>>>");
	fgets(msg.address,sizeof(msg.address),stdin);
	msg.address[strlen(msg.address)-1] = 0;
	printf("请输入员工的薪资>>>");
	scanf("%f",&msg.salary);
	while(getchar()!=10);
	printf("请输入员工的年龄>>>");
	scanf("%d",&msg.age);
	while(getchar()!=10);
	msg.type = 'I';
	if(send(sfd,&msg,sizeof(msg),0) < 0)
	{
		ERR_MSG("send");
		return -1;
	}
	
	if(recv(sfd,buf,1,0) < 0)
	{
		ERR_MSG("recv");
		return -1;
	}
	printf("%s\n",buf);
	if(strcmp(buf,"T")==0)
	{
		printf("新员工信息添加成功\n");
		return 0;
	}else if(strcmp(buf,"F") == 0)
	{
		printf("新员工信息添加失败\n");
		return -1;
	}
	return 0;
}

int Del_Employee(int sfd)
{
	//根据姓名+手机号删除
	char name[20] = "";
	int ipone;
	struct info msg;
	printf("请输入删除员工的姓名>>>");
	fgets(name,sizeof(name),stdin);
	name[strlen(name)-1] = 0;
	printf("请输入删除员工的手机号>>>");
	scanf("%d",&ipone);
	while(getchar()!=10);
	strcpy(msg.name,name);
	msg.ipone_num = ipone;
	msg.type = 'D';
	if(send(sfd,&msg,sizeof(msg),0)<0)
	{
		ERR_MSG("send");
		return -1;
	}
	bzero(name,sizeof(name));
	if(recv(sfd,&name,sizeof(name),0) < 0)
	{
		ERR_MSG("recv");
		return -1;
	}
	if(strcmp(name,"A") == 0)
	{
		char choose;
		printf("是否确认删除(y/n)");
		scanf("%c",&choose);
		while(getchar()!=10);
		if(send(sfd,&choose,sizeof(choose),0) < 0)
		{
			ERR_MSG("send");
			return -1;
		}
		bzero(name,sizeof(name));
		if(recv(sfd,name,1,0) < 0)
		{
			ERR_MSG("recv");
			return -1;
		}
		if(strcmp(name,"T") ==0)
		{
			printf("删除成功\n");
			return 0;
		}else if(strcmp(name,"F") == 0)
		{
			printf("删除失败\n");
			return 0;
		}
	}else if(strcmp(name,"B") == 0)
	{
		printf("没有该员工\n");
		return -1;
	}
	return 0;
}

int Modify_Employee(int sfd,struct info msg)
{
	//修改员工信息
	int ret;
	ret = Search_EMployee(sfd,msg);
	if(ret < 0)
		return 0;

//	printf("-------------------\n");
//	printf("%d\n",ret);
	msg.ipone_num = ret;
	msg.type = 'C';
	if(msg.flag == 1)
	{
		//管理员修改
		char choose;
		char name[20] = "";
		char address[20] = "";
		int ipone,age;
		float salary;
		int flag = 0;
		while(1)
		{	

			printf("******************************************\n");
			printf("***************1.修改姓名*****************\n");
			printf("***************2.修改手机号***************\n");
			printf("***************3.修改地址*****************\n");
			printf("***************4.修改薪资*****************\n");
			printf("***************5.修改年龄*****************\n");
			printf("***************6.退出*********************\n");
			printf("******************************************\n");
			printf("请输入选项>>>");
			scanf("%c",&choose);
			while(getchar()!=10);
			switch(choose)
			{
			case '1':
//				printf("%d\n",msg.ipone_num);
				printf("请输入新的名字>>>");
				fgets(name,sizeof(name),stdin);
				name[strlen(name)-1] = 0;
				strcpy(msg.name,name);
				msg.Mflag = 1;
				if(send(sfd,&msg,sizeof(msg),0)<0)
				{
					ERR_MSG("send");
					return -1;
				}
				bzero(name,sizeof(name));
				if(recv(sfd,name,1,0)<0)
				{
					ERR_MSG("recv");
					return -1;
				}
				if(strcmp(name,"T")==0)
				{
					printf("修改成功\n");
				}else{
					printf("修改失败\n");
				}

				break;
			case '2':
				printf("请输入新的手机号>>>");
				scanf("%d",&ipone);
				while(getchar()!=10);
				msg.newipone = ipone;
				msg.Mflag = 2;
				if(send(sfd,&msg,sizeof(msg),0)<0)
				{
					ERR_MSG("send");
					return -1;
				}
				bzero(name,sizeof(name));
				if(recv(sfd,name,1,0)<0)
				{
					ERR_MSG("recv");
					return -1;
				}
				if(strcmp(name,"T")==0)
				{
					printf("修改成功,请重新输入\n");
				}else{
					printf("修改失败\n");
				}

				return 0;
			case '3':
				printf("请输入新的地址>>>");
				fgets(address,sizeof(address),stdin);
				address[strlen(address)-1] = 0;
				strcpy(msg.address,address);
				msg.Mflag = 3;
				if(send(sfd,&msg,sizeof(msg),0)<0)
				{
					ERR_MSG("send");
					return -1;
				}
				bzero(name,sizeof(name));
				if(recv(sfd,name,1,0)<0)
				{
					ERR_MSG("recv");
					return -1;
				}
				if(strcmp(name,"T")==0)
				{
					printf("修改成功\n");
				}else{
					printf("修改失败\n");
				}

				break;
			case '4':
				printf("请输入新的薪资>>>");
				scanf("%f",&salary);
				while(getchar()!=10);
				msg.salary = salary;
				msg.Mflag = 4;
				if(send(sfd,&msg,sizeof(msg),0)<0)
				{
					ERR_MSG("send");
					return -1;
				}
				bzero(name,sizeof(name));
				if(recv(sfd,name,1,0)<0)
				{
					ERR_MSG("recv");
					return -1;
				}
				if(strcmp(name,"T")==0)
				{
					printf("修改成功\n");
				}else{
					printf("修改失败\n");
				}

				break;
			case '5':
				printf("请输入新的年龄>>>");
				scanf("%d",&age);
				while(getchar()!=10);
				msg.age = age;
				msg.Mflag = 5;
				if(send(sfd,&msg,sizeof(msg),0)<0)
				{
					ERR_MSG("send");
					return -1;
				}
				bzero(name,sizeof(name));
				if(recv(sfd,name,1,0)<0)
				{
					ERR_MSG("recv");
					return -1;
				}
				if(strcmp(name,"T")==0)
				{
					printf("修改成功\n");
				}else{
					printf("修改失败\n");
				}

				break;
			case '6':
				flag = 1;
				break;
			default:
				printf("输入错误\n");
				continue;
			}
			if(flag == 1)
			{
				printf("退出成功\n");
				break;
			}
			
		}
	}
	else{
		//员工修改
		char choose;
		char name[20] = "";
		char address[20] = "";
		int ipone;
		int flag = 0;

//		printf("%d\n",msg.ipone_num);
		while(1)
		{
			printf("******************************************\n");
			printf("***************1.修改姓名*****************\n");
			printf("***************2.修改年龄*****************\n");
			printf("***************3.修改地址*****************\n");
			printf("***************4.保存并退出***************\n");
			printf("******************************************\n");
			printf("请输入选项>>>");
			scanf("%c",&choose);
			while(getchar()!=10);
			switch(choose)
			{
			case '1':
				printf("请输入新的名字>>>");
				fgets(name,sizeof(name),stdin);
				name[strlen(name)-1] = 0;
				strcpy(msg.name,name);
				msg.Mflag = 1;
				if(send(sfd,&msg,sizeof(msg),0)<0)
				{
					ERR_MSG("send");
					return -1;
				}
				bzero(name,sizeof(name));
				if(recv(sfd,name,1,0)<0)
				{
					ERR_MSG("recv");
					return -1;
				}
				if(strcmp(name,"T")==0)
				{
					printf("修改成功\n");
				}else{
					printf("修改失败\n");
				}

				break;
			case '2':
				printf("请输入修改的年龄>>>");
				scanf("%d",&ipone);
				while(getchar()!=10);
				msg.age = ipone;
				msg.Mflag = 5;
				if(send(sfd,&msg,sizeof(msg),0)<0)
				{
					ERR_MSG("send");
					return -1;
				}
				bzero(name,sizeof(name));
				if(recv(sfd,name,1,0)<0)
				{
					ERR_MSG("recv");
					return -1;
				}
				if(strcmp(name,"T")==0)
				{
					printf("修改成功\n");
				}else{
					printf("修改失败\n");
				}

				break;
			case '3':
				printf("请输入新的地址>>>");
				fgets(address,sizeof(address),stdin);
				address[strlen(address)-1] = 0;
				strcpy(msg.address,address);
				msg.Mflag = 3;
				if(send(sfd,&msg,sizeof(msg),0)<0)
				{
					ERR_MSG("send");
					return -1;
				}
				bzero(name,sizeof(name));
				if(recv(sfd,name,1,0)<0)
				{
					ERR_MSG("recv");
					return -1;
				}
				if(strcmp(name,"T")==0)
				{
					printf("修改成功\n");
				}else{
					printf("修改失败\n");
				}

				break;
			case '4':
				flag = 1;
				break;
			default:
				printf("输入错误\n");
				continue;
			}
			if(flag == 1)
			{
				printf("退出成功\n");
				break;
			}
				}

	}
	return 0;
}
int Search_EMployee(int sfd,struct info msg)
{
	//查找
	int ipone;
	if(msg.flag == 1)
	{//管理员查找
		int i,j;
		char buf[128] = "";
//		struct info msg;
		printf("请输入查找/修改员工的手机号>>>");
		scanf("%d",&ipone);
		while(getchar()!=10);
		msg.ipone_num = ipone;
		msg.type = 'F';
		if(send(sfd,&msg,sizeof(msg),0)<0)
		{
			ERR_MSG("send");
			return -1;
		}
		if(recv(sfd,buf,1,0)<0)
		{
			ERR_MSG("recv");
			return -1;
		}
		if(strcmp(buf,"T") == 0)
		{
			bzero(buf,sizeof(buf));
			if(recv(sfd,buf,sizeof(buf),0)<0)
			{
				ERR_MSG("recv");
				return -1;
			}
			printf("--------------员工信息----------------\n");
			printf("%s\n",buf);
			bzero(buf,sizeof(buf));
			if(recv(sfd,buf,sizeof(buf),0)<0)
			{
				ERR_MSG("recv");
				return -1;
			}
			printf("%s\n\n",buf);
		}else if(strcmp(buf,"F") == 0)
		{
			printf("没有该员工\n");
			return -1;
		}
	}else{
		//员工查找
		char buf[128] = "";
		msg.type = 'F';

		ipone = msg.ipone_num;
		if(send(sfd,&msg,sizeof(msg),0)<0)
		{
			ERR_MSG("send");
			return -1;
		}
		if(recv(sfd,buf,1,0)<0)
		{
			ERR_MSG("recv");
			return -1;
		}
		if(strcmp(buf,"T") == 0)
		{
			bzero(buf,sizeof(buf));
			if(recv(sfd,buf,sizeof(buf),0)<0)
			{
				ERR_MSG("recv");
				return -1;
			}
			printf("--------------员工信息----------------\n");
			printf("%s\n",buf);
			bzero(buf,sizeof(buf));
			if(recv(sfd,buf,sizeof(buf),0)<0)
			{
				ERR_MSG("recv");
				return -1;
			}
			printf("%s\n\n",buf);
		}else if(strcmp(buf,"F") == 0)
		{
			printf("没有你的信息,请联系管理员\n");
			return -1;
		}

	}
	return ipone;
}
