#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define NAME_TO_STR(_name)         (#_name)

int dbg_level = LOG_LEVEL_ERROR;
char g_virtule_console[MAX_DEVICE_LEN+1];

char* LogLevelToStr(int level)
{
	char* log_level_str[LOG_LEVEL_MAX] = {
		[LOG_LEVEL_ERROR] = "LOG_LEVEL_ERROR",
		[LOG_LEVEL_WARN]  = "LOG_LEVEL_WARN",
		[LOG_LEVEL_INFO]  = "LOG_LEVEL_INFO",
		[LOG_LEVEL_DEBUG] = "LOG_LEVEL_DEBUG"
	};

	if(level<LOG_LEVEL_ERROR)
		return log_level_str[LOG_LEVEL_ERROR];

	if(level>LOG_LEVEL_DEBUG)
		return log_level_str[LOG_LEVEL_DEBUG];

	return log_level_str[level];
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
	snprintf(virtule_console,MAX_DEVICE_LEN,"%s%s","/dev/",g_virtule_console);
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
