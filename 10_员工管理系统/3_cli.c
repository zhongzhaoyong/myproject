#include "func.h"
int main(int argc,const char *argv[])
{
	int sfd;
	//网络初始化
	sfd = cli_inet_init();
	if(sfd < 0)
		return -1;
	//登录系统
	Login_System(sfd);
	return 0;
}
