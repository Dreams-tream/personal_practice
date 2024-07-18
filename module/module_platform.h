#ifndef _MODULE_PLATFORM_H
#define _MODULE_PLATFORM_H

#include "module_common.h"

#define MODULE_CODE_DIR                         "/home/yangcuilang/GIT_PROJECT/personal_practice/module"
#define DEFAULT_AUTHOR_NAME                     "Yang Cuilang"
#define CONFIG_FILE_POSTFIX                     ".cfg"
#define AUTHOR_NAME_LEN                         30
#define MODULE_FILE_LEN                         100
#define LOOP_TIMEOUT_SECOND                     0/*s*/
#define LOOP_TIMEOUT_MILLISECOND                (500) /*ms*/
#define LOOP_TIMEOUT_MICROSECOND                LOOP_TIMEOUT_MILLISECOND*1000  /*us*/

#ifdef CONFIG_AUTHOR_NAME_SUPPORT
#define MODULE_PRE_CONFIG                  MODULE_CODE_DIR"module.config"
#define CONFIG_AUTHOR_NAME                 "CONFIG_AUTHOR_NAME="
#define MODULE_PRE_CONFIG_FILE             MODULE_CODE_DIR"MODULE.cfg"
#endif

#define void_func(_func)               void _func(void)
#define void_ptr_func(_func)           void* _func(void)
#define int_func(_func)                int _func(void)
#define int_ptr_func(_func)            int* _func(void)

typedef enum _event_type{
	EVENT_TYPE_MIN,
	EVENT_TYPE_MAX,
}event_type;

typedef struct _module_config{
	int second;
	int millisecond;
	char author[AUTHOR_NAME_LEN+1];
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

