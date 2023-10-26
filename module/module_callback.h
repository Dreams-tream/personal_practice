#ifndef _MODULE_CALLBACK_H
#define _MODULE_CALLBACK_H

#include"module_platform.h"

void module_init_platform(module_callback *cb);
void module_timer_loop(module_callback *cb);
void module_event_loop(module_callback *cb);
void module_exit(module_cfg *cfg);


#endif
