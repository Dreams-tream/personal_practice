#include "log.h"
#include "string.h"
#include "module_shell.h"


/***************************************MACRO*******************************************/
#define NET_DIR                                "/sys/class/net/"


/*************************************FUNCTIONS*****************************************/
STATUS shell_get_ip_mac_mask_address(char ipaddr[], int len)
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

