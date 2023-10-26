#include"module_callback.h"
#include<stdio.h>

void module_init_platform(module_callback *cb)
{
	if(cb->init_platform)
		cb->init_platform();
}
void module_event_loop(module_callback *cb)
{
	if(cb->event_loop)
		cb->event_loop();
}

void module_timer_loop(module_callback *cb)
{
	if(cb->timer_loop)
		cb->timer_loop(cb);
}

void module_exit(module_cfg *cfg)
{
	if(cfg->cb.exit)
		cfg->cb.exit(cfg->conf);
}

void module_register_callbacks(module_callback *cb)
{
	cb->init_platform = platform_init_platform;
	cb->event_loop    = platform_event_loop;
	cb->timer_loop    = platform_timer_loop;
	cb->exit          = platform_exit;
}
