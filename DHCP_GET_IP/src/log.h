#ifndef _LOG_H
#define _LOG_H

enum{
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG
};

#define MAX_CMD_LEN 256

void va_printf(const char* fmt, ...);
void _exec_get_res(const char* cmd,char *res);

#define exec_get_res(cmd,buf) do{\
	CMD(cmd);\
	_exec_get_res(cmd,buf);\
}while(0)

#define LOG_ERR(fmt, ...) do{\
	if ( dbg_level >= LOG_LEVEL_ERROR )\
	{\
		va_printf("LOG_ERR[%s,%d]" fmt "\n",__func__,__LINE__,##__VA_ARGS__);\
	}\
}while(0)

#define LOG_WARN(fmt,arg...) do{\
	if ( dbg_level >= LOG_LEVEL_WARN )\
		va_printf("LOG_WARN[%s,%d]"fmt"\n",__func__,__LINE__,##arg);\
}while(0)

#define LOG_INFO(fmt,arg...) do{\
	if ( dbg_level >= LOG_LEVEL_INFO )\
	va_printf("LOG_INFO[%s,%d]"fmt"\n",__func__,__LINE__,##arg);\
}while(0)

#define LOG_DEBUG(fmt,arg...) do{\
	if ( dbg_level >= LOG_LEVEL_DEBUG )\
		va_printf("LOG_DEBUG[%s,%d]"fmt"\n",__func__,__LINE__,##arg);\
}while(0)

#define CMD(cmd) do{\
	printf("CMD[%s,%d]%s\n",__func__,__LINE__,cmd);\
}while(0)

#endif
