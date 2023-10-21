#ifndef _MODULE_CALLBACK_H
#define _MODULE_CALLBACK_H


typedef void_func(timer_loop_callback);
typedef void_func(event_loop_callback, event_type);


typedef struct module_callback{
	timer_loop_callback timer_loop;
	event_loop_callback event_loop;
};

#endif
