#ifndef _MODULE_PLATFORM_H
#define _MODULE_PLATFORM_H

#define AUTHOR_NAME_LEN                         30

#define void_func(_func,_args...)               void _func(##args)
#define void_ptr_func(_func,_args...)           void* _func(##args)
#define int_func(_func,_args...)                int _func(##args)
#define int_ptr_func(_func,_args...)            int* _func(##args)


typedef struct module_config{
	char author[AUTHOR_NAME_LEN+1];
	unsigned int times;
};

int_func(module_load_config);
void_func(module_init_platform);
void_func(module_timer_loop);
void_func(module_event_loop);



#endif

