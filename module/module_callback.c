#include"module_callback.h"
#include"module_platform.h"
#include<stdio.h>

void module_init_platform(module_callback *cb)
{
	cb->init_platform();
}
void module_event_loop(module_callback *cb)
{
	cb->event_loop();
}

void module_timer_loop(module_callback *cb)
{
	cb->timer_loop();
}

void module_register_callbacks(module_callback *cb)
{
	cb->init_platform = platform_init_platform;
	cb->event_loop    = platform_event_loop;
	cb->timer_loop    = platform_timer_loop;
}
