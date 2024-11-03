#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "log.h"
#include "module_ioctl.h"


/***************************************MACRO*******************************************/
#define DO_SOMETHING_COUNT                                  5

/**********************************GLOBAL VARIABLE**************************************/
static int timer_cnt = 0;
static int wait_cnt = 0;

/*********************************** FUNCTIONS *****************************************/
static STATUS shell_get_ip_mac_mask_address(char ipaddr[], int len)
{
	char cmd[MAX_CMD_LEN] = {0};
	char buf[MAX_CMD_LEN] = {0};

	if (!ipaddr || len < 0)
	{
		return ERROR;
	}

	strncpy(cmd, "ifconfig |grep 'inet addr'|grep Bcast|gawk '{print $2}'", MAX_CMD_LEN);
	if (ERROR == module_exec_get_res(cmd, buf, sizeof(buf)))
	{
		return ERROR;
	}

	strncpy(ipaddr, buf + 5, len - 1);
	return OK;
}

static STATUS get_ip_addr(char ipaddr[], int len)
{
	return
#ifdef USE_IOCTL
	ioctl_get_ip_mac_mask_address(ipaddr, len);
#else
	shell_get_ip_mac_mask_address(ipaddr, len);
#endif
}

void check_ip_address()
{
	char ipaddr[MAX_CMD_LEN] = {0};

	if(wait_cnt)
	{
		wait_cnt--;
		return;
	}

	if (OK == get_ip_addr(ipaddr, sizeof(ipaddr)))
	{
		LOG_INFO("your ipv4 address is %s", ipaddr);
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

