#ifndef _TYPE_H
#define _TYPE_H

enum event_type
{
	EVENT_TCP,
	EVENT_UDP,
	EVENT_DATA,
	EVENT_ACTION,
	EVENT_MANAGE,
};

static inline char *enum_to_str(enum event_type event)
{
#define STR(x) case(x): return #x
	switch(event)
	{
		STR(EVENT_TCP);
		STR(EVENT_UDP);
		STR(EVENT_DATA);
		STR(EVENT_ACTION);
		STR(EVENT_MANAGE);
	}
#undef STR
}

#endif
