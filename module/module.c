#include<stdio.h>
#include<signal.h>
#include <sys/types.h>
#include <unistd.h>

#include"module.h"
#include"log.h"

#define MODULE_PID_FILE             "/tmp/YCL.pid"

module_cfg g_module_cfg;


void_func(monitor_signal);
void_func(signal_process, int sig);
int_func(create_pid_file);

void_func(monitor_signal)
{
	signal(SIGINT,  signal_process);
	signal(SIGTERM, signal_process);
	signal(SIGUSR1, signal_process);
	signal(SIGUSR2, signal_process);
}

void_func(signal_process, int sig)
{
	switch(sig)
	{
	case SIGTERM:
	case SIGINT:
		LOG_ERR("Exit this module");
		exit(0);
	case SIGUSR1:
		dbg_level<LOG_LEVEL_DEBUG?dbg_level++:0;
		LOG_ERR("Increase log level to %s", LogLevelToStr(dbg_level));
		break;
	case SIGUSR2:
		dbg_level>LOG_LEVEL_ERROR?dbg_level--:0;
		LOG_ERR("Decrease log level to %s", LogLevelToStr(dbg_level));
		break;
	default:
		LOG_ERR("Unknown signal, break");
		break;
	}
}

int_func(create_pid_file)
{
	FILE *fp = fopen(MODULE_PID_FILE, "w+");
	pid_t pid = getpid();

	if(!fp)
	{

		return ERROR;
	}

	fprintf(fp,"%u",pid);
	fclose(fp);s
	return OK;
}

int_func(create_pid_file);

int main()
{
	monitor_signal();

	if(ERROR==create_pid_file())
	{
		LOG_ERR("Create pid file error!");
		return ERROR;
	}
	memset(&g_module_cfg,0,sizeof(g_module_cfg));
	module_load_config();
	module_register_callbacks(&g_module_cfg.cb);
	module_init_platform();
	module_timer_loop();
	return 0;
}
