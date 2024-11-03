#ifndef _MODULE_COMMON_H
#define _MODULE_COMMON_H

typedef enum{
	ERROR = -1,
	OK = 0,
} STATUS;

typedef enum{
	false,
	true,
} bool;

int str_replace(char *str,const char source,const char dest);

#endif
