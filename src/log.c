#include "log.h"

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
		ERR("popen failed!");
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
