#include <stdarg.h>
#include<string.h>
#include "log.h"



int dbg_level = LOG_LEVEL_ERROR;

enum{
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_MAX
};
#define NAME_TO_STR(_name)         #_name
char* LogLevelToStr(int dbg_level)
{
	char res[20] = {0};
	char* log_level_str[LOG_LEVEL_MAX] = {
		[LOG_LEVEL_ERROR] = "LOG_LEVEL_ERROR",
		[LOG_LEVEL_WARN]  = "LOG_LEVEL_WARN",
		[LOG_LEVEL_INFO]  = "LOG_LEVEL_INFO",
		[LOG_LEVEL_DEBUG] = "LOG_LEVEL_DEBUG"
	};

	if(dbg_level<LOG_LEVEL_ERROR || dbg_level>LOG_LEVEL_DEBUG)
	{
		return NULL;
	}

	snprintf(res,sizeof(res)-1,"%s",NAME_TO_STR(log_level_str[dbg_level]));
	return res;
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
