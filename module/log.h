#ifndef _LOG_H
#define _LOG_H

enum{
	LOG_LEVEL_ERROR = 0,
	LOG_LEVEL_WARN = 1,
	LOG_LEVEL_INFO = 2,
	LOG_LEVEL_DEBUG = 3,
	LOG_LEVEL_MAX
};


#define MAX_CMD_LEN            256
#define MAX_DEVICE_LEN         50

int get_log_level();
void va_printf(const char* fmt, ...);
void va_exec_cmd(const char* fmt, ...);
int module_exec_get_res(const char *cmd,char *buf, int buflen);

#define module_exec_cmd(fmt, ...) do{\
	va_printf("[%s, %d]" fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	va_exec_cmd(fmt, ##__VA_ARGS__);\
}while(0)

#define PRINT(fmt,args...) va_printf(fmt"\n",##args)

#define LOG_ERR(fmt, ...) do{\
	if ( get_log_level() >= LOG_LEVEL_ERROR )\
	{\
		va_printf("[%s, %d]" fmt "\n", __func__, __LINE__, ##__VA_ARGS__);\
	}\
}while(0)

#define LOG_WARN(fmt,arg...) do{\
	if ( get_log_level() >= LOG_LEVEL_WARN )\
		va_printf("[%s, %d]" fmt "\n", __func__, __LINE__, ##arg);\
}while(0)

#define LOG_INFO(fmt,arg...) do{\
	if ( get_log_level() >= LOG_LEVEL_INFO )\
	va_printf("[%s, %d]" fmt "\n", __func__, __LINE__, ##arg);\
}while(0)

#define LOG_DEBUG(fmt,arg...) do{\
	if ( get_log_level() >= LOG_LEVEL_DEBUG )\
		va_printf("[%s, %d]" fmt "\n", __func__, __LINE__, ##arg);\
}while(0)

void modify_log_level(int sig, int log_level);
char* LogLevelToStr(int dbg_level);

#endif
