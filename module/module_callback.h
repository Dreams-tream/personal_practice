#ifndef _MODULE_CALLBACK_H
#define _MODULE_CALLBACK_H


typedef void (*timer_loop_callback)();
typedef void (*event_loop_callback)(event_type);

typedef struct module_callback{
	timer_loop_callback timer_loop;
	event_loop_callback event_loop;
}module_callback;

#endif
