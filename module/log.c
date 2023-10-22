#include <stdarg.h>
#include<string.h>
#include "log.h"

#define NAME_TO_STR(_name)         (#_name)

int dbg_level = LOG_LEVEL_MAX;

char* LogLevelToStr(int level)
{
	char res[20] = {0};
	char* log_level_str[LOG_LEVEL_MAX] = {
		[LOG_LEVEL_ERROR] = "LOG_LEVEL_ERROR",
		[LOG_LEVEL_WARN]  = "LOG_LEVEL_WARN",
		[LOG_LEVEL_INFO]  = "LOG_LEVEL_INFO",
		[LOG_LEVEL_DEBUG] = "LOG_LEVEL_DEBUG"
	};

	if(level<LOG_LEVEL_ERROR || level>LOG_LEVEL_DEBUG)
	{
		return NULL;
	}

	snprintf(res,sizeof(res)-1,"%s",NAME_TO_STR(log_level_str[level]));
	return res;
}

void va_exec_cmd(const char* fmt, ...)
{
	va_list list;
	char buffer[MAX_CMD_LEN+1] = {0};
	va_start(list,fmt);
	vsprintf(buffer,fmt,list);
	system(buffer);
}

/*print in varieties of formats*/
void va_printf(const char* fmt, ...)
{
	va_list args;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);
}

/*execute cmd and get all output in res if the length of result is less than MAX_CMD_LEN*/
void _exec_get_res(const char *cmd,char *res)
{
	FILE *fp = NULL;
	char *p = NULL;

	if ( NULL == (fp=popen(cmd,"r")) )
	{
		LOG_ERR("popen failed!");
		return;
	}

	char *str = res;
	while( NULL != fgets(str,MAX_CMD_LEN,fp) )//fgets add '\0' at the end
	{
		str+=strlen(str);
	}
	p = &str[strlen(str)-1];
	if ( *p == '\n' || *p == '\r' )
		*p = '\0';
	fclose(fp);
}
