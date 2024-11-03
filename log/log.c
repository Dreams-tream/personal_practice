#include<stdio.h>
#include<stdlib.h>
#include"log.h"

void _cmd(const char *fmt, ...)
{
	char buf[1000];

	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	system(buf);
}
