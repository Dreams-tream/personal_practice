#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define NAME_TO_STR(_name)         (#_name)

static int s_log_level = LOG_LEVEL_ERROR;
char g_virtule_console[MAX_DEVICE_LEN+1];

int get_log_level()
{
	return s_log_level;
}

void modify_log_level(int sig, int log_level)
{
	if(SIGUSR1==sig)
	{
		s_log_level<LOG_LEVEL_DEBUG?s_log_level++:0;
		LOG_ERR("Increase log level to %s", LogLevelToStr(s_log_level));
	}else if(SIGUSR2==sig)
	{
		s_log_level>LOG_LEVEL_ERROR?s_log_level--:0;
		LOG_ERR("Decrease log level to %s", LogLevelToStr(s_log_level));
	}else
	{
		s_log_level = (log_level>LOG_LEVEL_DEBUG?LOG_LEVEL_DEBUG:\
			(log_level<LOG_LEVEL_ERROR?LOG_LEVEL_ERROR:log_level));
		LOG_ERR("Set log level to %s", LogLevelToStr(s_log_level));
	}
}

char* LogLevelToStr(int log_level)
{
	char* log_level_str[LOG_LEVEL_MAX] = {
		[LOG_LEVEL_ERROR] = "LOG_LEVEL_ERROR",
		[LOG_LEVEL_WARN]  = "LOG_LEVEL_WARN",
		[LOG_LEVEL_INFO]  = "LOG_LEVEL_INFO",
		[LOG_LEVEL_DEBUG] = "LOG_LEVEL_DEBUG"
	};

	if(log_level<LOG_LEVEL_ERROR)
		return log_level_str[LOG_LEVEL_ERROR];

	if(log_level>LOG_LEVEL_DEBUG)
		return log_level_str[LOG_LEVEL_DEBUG];

	return log_level_str[log_level];
}

void va_exec_cmd(const char* fmt, ...)
{
	va_list list;
	char buffer[MAX_CMD_LEN+1] = {0};
	va_start(list,fmt);
	vsnprintf(buffer,MAX_CMD_LEN,fmt,list);
	va_end(list);
	system(buffer);
}

/*print in varieties of formats*/
void va_printf(const char* fmt, ...)
{
	int fd;
	char buffer[MAX_CMD_LEN+1] = {0};
	char virtule_console[MAX_DEVICE_LEN+1] = {0};
	va_list args;
	va_start(args,fmt);
	vsnprintf(buffer,MAX_CMD_LEN,fmt,args);
	va_end(args);
	/*In fact, /dev/tty points to current terminal*/
	snprintf(virtule_console,MAX_DEVICE_LEN,"%s%s","/dev/","tty"/*g_virtule_console*/);
	if((fd=open(virtule_console,O_RDWR))<=0)
	{
		perror("open console failed!");
		return;
	}
	write(fd,buffer, strlen(buffer));
	close(fd);
}

/*execute cmd and get all output in res if the length of result is less than MAX_CMD_LEN*/
int module_exec_get_res(const char *cmd,char *res)
{
	FILE *fp = NULL;
	char *p = NULL;

	//printf("%s\n",cmd);
	if ( NULL == (fp=popen(cmd,"r")) )
	{
		LOG_ERR("popen failed!");
		return (-1);
	}

	char *str = res;
	while( NULL != fgets(str,MAX_CMD_LEN,fp) )//fgets add '\0' at the end
	{
		str+=strlen(str);
	}
	p = &str[strlen(str)-1];
	if ( *p == '\n' || *p == '\r' )
		*p = '\0';
	pclose(fp);
	//printf("%s\n",res);
	return 0;
}
