#ifndef _MODULE_CALLBACK_H
#define _MODULE_CALLBACK_H

#include"module_callback.h"
#include"module_platform.h"



enum{
	ERROR = -1,
	OK = 0,
};

typedef enum event_type{


};
typedef struct module_cfg{
	module_config   conf;
	module_callback cb;
};





#endif
