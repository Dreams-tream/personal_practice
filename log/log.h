#ifndef _LOG_H
#define _LOG_H

#include<stdarg.h>

typedef enum
{
	ERROR = -1,
	OK = 0,
} STATUS;

typedef enum
{
	false,
	true,
} bool;

inline STATUS va_print() __attribute__((always_inline));
void _cmd(const char *fmt, ...);

#define LOG(fmt, args...) printf("[%s,%d]" fmt "\n", __func__, __LINE__, ##args)
#define STR(x) #x
#define CMD(fmt, args...) LOG(fmt, ##args);_cmd(fmt, ##args); 

#endif
