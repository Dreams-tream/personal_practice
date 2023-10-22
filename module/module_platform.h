#ifndef _MODULE_PLATFORM_H
#define _MODULE_PLATFORM_H

#define MODULE_CODE_DIR                         "/home/yangcuilang/C_Code/module/"
#define DEFAULT_AUTHOR_NAME                     "YCL"
#define CONFIG_FILE_POSTFIX                     ".cfg"
#define AUTHOR_NAME_LEN                         30
#define MODULE_FILE_LEN                         100

#define void_func(_func)               void _func(void)
#define void_ptr_func(_func)           void* _func(void)
#define int_func(_func)                int _func(void)
#define int_ptr_func(_func)            int* _func(void)

typedef struct module_config{
	char author[AUTHOR_NAME_LEN+1];
	int seconds;
}module_config;

enum{
	ERROR = -1,
	OK = 0,
};


void_func(platform_init_platform);
void_func(platform_timer_loop);
void_func(platform_event_loop);


#endif

