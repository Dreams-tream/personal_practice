#include "platform.h"
#include <stdio.h>
#include <string.h>
#include "log.h"

#define DO_SOMETHING_COUNT                                  5

int timer_cnt = 0;
int wait_cnt = 0;

/*********************************** FUNCTIONS *****************************************/
void check_ip_address()
{
	char cmd[MAX_CMD_LEN] = {0};
	char buf[MAX_CMD_LEN] = {0};

	if(wait_cnt)
	{
		wait_cnt--;
		return;
	}

	strncpy(cmd, "ifconfig |grep 'inet addr'|grep Bcast|gawk '{print $2}'", MAX_CMD_LEN);

	if (!module_exec_get_res(cmd, buf, sizeof(buf)))
	{
		LOG_DEBUG("buf is '%s'", buf);
		LOG_INFO("your ipv4 address is %s", buf + 5);
	}
	else
	{
		LOG_ERR("you don't have ipv4 address now\nstart to get ip address!");
		module_exec_cmd("sudo dhclient -v");
		wait_cnt = 5;
	}
}

void timer_do_something()
{
	if(!timer_cnt)
	{
		check_ip_address();
		timer_cnt = DO_SOMETHING_COUNT;
	}
	else
		timer_cnt--;
}

