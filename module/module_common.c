#include "module_common.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int str_replace(char *str,const char source,const char dest)
{
	int i = 0;

	if(!str || !isascii(source) || !isascii(dest))
		return ERROR;

	for(i=0;i<strlen(str);i++)
	{
		if(source==str[i])
			str[i] = dest;
	}
	return OK;
}
