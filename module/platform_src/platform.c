#include "platform.h"
#include <stdio.h>
#include <string.h>
#include "../log.h"

#define DO_SOMETHING_COUNT                                  5

void check_ip_address();

void check_ip_address()
{
	int need_dhcp = 0;
	static int wait_cnt = 0;
	char cmd[MAX_CMD_LEN+1]={0};
	char buf[MAX_CMD_LEN+1]={0};

	if(wait_cnt)
	{
		wait_cnt--;
		return;
	}
	strncpy(cmd,"ifconfig |grep 'inet addr'|grep Bcast|gawk '{print $2}'",MAX_CMD_LEN);
	module_exec_get_res(cmd,buf);
	LOG_DEBUG("buf is %s",buf);
	if(strlen(buf) && buf[0]!='\n' && buf[0]!='r' )
	{
		LOG_INFO("your ipv4 address is %s",buf+5);
		need_dhcp = 0;
	}
	else
	{
		LOG_ERR("you don't have ipv4 address now\nstart to get ip address!");
		need_dhcp = 1;
	}

	if(need_dhcp)
	{
		module_exec_cmd("sudo dhclient -v");
		wait_cnt = 5;
	}
}

void timer_do_something()
{
	static int timer_cnt=0;

	if(!timer_cnt)
	{
		check_ip_address();
		timer_cnt = DO_SOMETHING_COUNT;
	}
	else
		timer_cnt--;
}

