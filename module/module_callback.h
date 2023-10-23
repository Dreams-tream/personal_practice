#ifndef _MODULE_CALLBACK_H
#define _MODULE_CALLBACK_H

typedef void (*init_platform_callback)();
typedef void (*timer_loop_callback)();
typedef void (*event_loop_callback)(event_type);

typedef struct module_callback{
	init_platform_callback    init_platform;
	timer_loop_callback       timer_loop;
	event_loop_callback       event_loop;
	/*void (*event_loop)();*/
}module_callback;

void module_init_platform(module_callback *cb);
void module_timer_loop(module_callback *cb);
void module_event_loop(module_callback *cb);

#endif
