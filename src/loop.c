#include <stdio.h>
#include "loop.h"
#include <string.h>

#define SLEEP_INTERVAL 2

void timer_loop()
{
	int cnt = 0;
	int need_dhcp = 0;
	char cmd[MAX_CMD_LEN]={0};
	char buf[MAX_CMD_LEN]={0};
	ERR("\n");
	for(;;)
	{
		strncpy(cmd,"ifconfig |grep 'inet addr'|grep Bcast|gawk '{print $2}'",sizeof(cmd));
		memset(buf,0,sizeof(buf));
		WARN("the software sleep %d second each time",SLEEP_INTERVAL);
		sleep(SLEEP_INTERVAL);
		cnt+=1;
		LOG("cnt=%d",cnt);
		exec_get_res(cmd,buf);
		INFO("buf is %s--------------------",buf);
		if(strlen(buf) && buf[0]!='\n' && buf[0]!='r' )
		{
			ERR("your ipv4 address is %s",buf+5);
			need_dhcp = 0;
		}
		else
		{
			ERR("you don't have ipv4 address now\nstart to get ip address!");
			need_dhcp = 1;
			sleep(1);
		}

		if( need_dhcp )
		{
			strncpy(cmd,"sudo dhclient -v",sizeof(cmd));
			CMD(cmd);
			sleep(2);
			system("echo '----------------------------------------'");
			system(cmd);
			sleep(5);
			system("echo '----------------------------------------'");
		}
	}
}
