#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "loop.h"

extern int dbg_level;

void exit_process(int sig)
{
	if(SIGINT == sig || SIGTERM == sig)
	{
		LOG_ERR("exit the program!\n");
		exit(0);
	}
}

void adjust_debug_level(int sig)
{

	if(SIGUSR1 == sig )
		dbg_level < LOG_LEVEL_DEBUG ? dbg_level++ : 0;
	if(SIGUSR2 == sig )
		dbg_level > LOG_LEVEL_ERROR ? dbg_level-- :0;

	LOG_ERR("dbg_level=%d\n",dbg_level);
}

void _signal_receive()
{
	signal(SIGTERM, exit_process);
	signal(SIGINT,  exit_process);
	signal(SIGUSR1, adjust_debug_level);
	signal(SIGUSR2, adjust_debug_level);
}

int main()
{
	LOG_ERR("this is the start of function");
	_signal_receive();
	timer_loop();
	return 0;
}
