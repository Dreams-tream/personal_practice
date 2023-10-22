#include <stdio.h>
#ifndef _LOG_H
#define _LOG_H

enum{
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_MAX
};


#define MAX_CMD_LEN 256

void va_printf(const char* fmt, ...);
void va_exec_cmd(const char* fmt, ...);

#define module_exec_cmd(fmt,...) do{\
	va_printf("CMD[%s,%d]"fmt"\n",__FUNCTION__,__LINE__,##__VA_ARGS__);\
	va_exec_cmd(fmt,##__VA_ARGS__);\
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

char* LogLevelToStr(int dbg_level);

#endif
