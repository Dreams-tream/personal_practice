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

enum{
	ERROR = -1,
	OK = 0,
};

typedef enum _event_type{
	EVENT_TYPE_MIN,
	EVENT_TYPE_MAX,
}event_type;

typedef struct _module_config{
	char author[AUTHOR_NAME_LEN+1];
	int period;
}module_config;

typedef struct _module_callback{
	void (*init_platform)();
	void (*timer_loop)(module_config);
	void (*event_loop)();
	void (*exit)(module_config);
}module_callback;

typedef struct _module_cfg{
	module_config   conf;
	module_callback cb;
}module_cfg;

void_func(platform_init_platform);
void platform_timer_loop(module_config);
void_func(platform_event_loop);
void platform_exit(module_config);



#endif

