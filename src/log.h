#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/*define 4 print levels, you can send SIGUSR1 or SIGUSR2 to adjust current level*/
enum{
	DBG_ERR,
	DBG_WARN,
	DBG_INFO,
	DBG_LOG
};

enum{
	NOK = -1,
	OK  = 0
};

#define MAX_CMD_LEN 1024

extern int dbg_level;

void va_printf(const char* fmt, ...);
void _exec_get_res(const char* cmd,char *res);

#define exec_get_res(cmd,buf) do{\
	CMD(cmd);\
	_exec_get_res(cmd,buf);\
}while(0)

#define ERR(fmt, ...) do{\
	if ( dbg_level >= DBG_ERR )\
	{\
		va_printf("ERR[%s,%d]" fmt "\n",__func__,__LINE__,##__VA_ARGS__);\
	}\
}while(0)

#define WARN(fmt,arg...) do{\
	if ( dbg_level >= DBG_WARN )\
		va_printf("WARN[%s,%d]"fmt"\n",__func__,__LINE__,##arg);\
}while(0)

#define INFO(fmt,arg...) do{\
	if ( dbg_level >= DBG_INFO )\
	va_printf("WARN[%s,%d]"fmt"\n",__func__,__LINE__,##arg);\
}while(0)

#define LOG(fmt,arg...) do{\
	if ( dbg_level >= DBG_LOG )\
		va_printf("[%s,%d]"fmt"\n",__func__,__LINE__,##arg);\
}while(0)

#define CMD(cmd) do{\
	printf("CMD[%s,%d]%s\n",__func__,__LINE__,cmd);\
}while(0)
