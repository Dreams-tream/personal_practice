#ifndef _LOG_H
#define _LOG_H

enum{
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_MAX
};


#define MAX_CMD_LEN            256
#define MAX_DEVICE_LEN         50

void va_printf(const char* fmt, ...);
void va_exec_cmd(const char* fmt, ...);
int module_exec_get_res(const char *cmd,char *res);

#define module_exec_cmd(fmt,...) do{\
	va_printf("[%s,%d]"fmt"\n",__FUNCTION__,__LINE__,##__VA_ARGS__);\
	va_exec_cmd(fmt,##__VA_ARGS__);\
}while(0)

#define PRINT(fmt,args...) va_printf("[%s,%d]"fmt"\n",__func__,__LINE__,##args)

#define LOG_ERR(fmt, ...) do{\
	if ( dbg_level >= LOG_LEVEL_ERROR )\
	{\
		va_printf("[%s,%d]" fmt "\n",__func__,__LINE__,##__VA_ARGS__);\
	}\
}while(0)

#define LOG_WARN(fmt,arg...) do{\
	if ( dbg_level >= LOG_LEVEL_WARN )\
		va_printf("[%s,%d]"fmt"\n",__func__,__LINE__,##arg);\
}while(0)

#define LOG_INFO(fmt,arg...) do{\
	if ( dbg_level >= LOG_LEVEL_INFO )\
	va_printf("[%s,%d]"fmt"\n",__func__,__LINE__,##arg);\
}while(0)

#define LOG_DEBUG(fmt,arg...) do{\
	if ( dbg_level >= LOG_LEVEL_DEBUG )\
		va_printf("[%s,%d]"fmt"\n",__func__,__LINE__,##arg);\
}while(0)

char* LogLevelToStr(int dbg_level);

#endif
