#ifndef _MODULE_H
#define _MODULE_H

#include"module_platform.h"
#include"module_callback.h"

typedef enum event_type{
	EVENT_TYPE_MIN,
	EVENT_TYPE_MAX,
}event_type;

typedef struct module_cfg{
	module_config   conf;
	module_callback cb;
}module_cfg;


#endif
