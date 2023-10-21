#include"module_callback.h"
#include"module_platform.h"
#include<stdio.h>





void_func(module_register_callbacks, module_callback *cb)
{
	cb->event_loop = module_event_loop;
	cb->timer_loop = module_timer_loop;
}
