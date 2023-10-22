#include"module_platform.h"
#include <sys/timerfd.h>
#include <time.h>
//#include <unistd.h>
//#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>        /* Definition of uint64_t */
#include"log.h"

extern int dbg_level;

void platform_init_platform()
{

}

void platform_timer_loop()
{
	struct timespec now;
	struct itimerspec new_time;
	int timer_fd;
	uint64_t total_time,expire_time;
	int size_64bits = sizeof(uint64_t);

	LOG_INFO("Start timer loop");
	if (clock_gettime(CLOCK_REALTIME, &now) == -1)
	{
		LOG_ERR("get current time failed");
		goto EXIT;
	}

	if(-1 == (timer_fd = timerfd_create(CLOCK_REALTIME,0)))
	{
		LOG_ERR("Create timer file descriptor failed");
		goto EXIT;
	}

	new_time.it_value.tv_sec = now.tv_sec;
	new_time.it_value.tv_nsec = now.tv_nsec;
	new_time.it_interval.tv_sec = 1;
	new_time.it_interval.tv_nsec= 0;
	if(-1 == timerfd_settime(timer_fd,TFD_TIMER_ABSTIME, &new_time, NULL))
	{
		LOG_ERR("set new time failed");
		goto EXIT;
	}

	for(total_time=0;;)
	{
		if(size_64bits != read(timer_fd,&expire_time,sizeof(uint64_t)))
		{
			LOG_ERR("Read timer failed");
			goto EXIT;
		}
		total_time += expire_time;
		LOG_INFO("total_time=%llu, expire_time=%llu",
			(unsigned long long)total_time,(unsigned long long)expire_time);
	}
	exit(OK);
EXIT:
	exit(ERROR);
}

void platform_event_loop()
{

}


