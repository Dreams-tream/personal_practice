#include"module_platform.h"
#include <sys/select.h>
#include <sys/timerfd.h>
#include <time.h>
//#include <unistd.h>
//#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>        /* Definition of uint64_t */
#include"log.h"

#define LOOP_TIMEOUT_MILLISECOND                       (500) /*ms*/
#define LOOP_TIMEOUT_MICROSECOND                       LOOP_TIMEOUT_MILLISECOND*1000  /*us*/

extern int dbg_level;

void platform_init_platform()
{

}

void platform_timer_loop()
{
	int timer_fd;
	int max_fd = 0;
	int size_64bits = sizeof(uint64_t);
	int select_res = -1;
	fd_set readset;
	uint64_t total_time,expire_time;
	struct timespec now;
	struct itimerspec new_time;
	struct timeval timeout;

	LOG_INFO("Start timer loop");
	//Get realtime clock
	if (clock_gettime(CLOCK_REALTIME, &now) == -1)
	{
		LOG_ERR("get current time failed");
		goto EXIT;
	}

	//create a timer file descriptor
	if(-1 == (timer_fd = timerfd_create(CLOCK_REALTIME,0)))
	{
		LOG_ERR("Create timer file descriptor failed");
		goto EXIT;
	}

	max_fd = (max_fd>timer_fd?max_fd:timer_fd);
	new_time.it_value.tv_sec = now.tv_sec;
	new_time.it_value.tv_nsec = now.tv_nsec;
	new_time.it_interval.tv_sec = 0;
	new_time.it_interval.tv_nsec= LOOP_TIMEOUT_MICROSECOND * 1000;
	timeout.tv_sec = 0;
	timeout.tv_usec = LOOP_TIMEOUT_MICROSECOND;

	//set timeout to control time
	if(-1 == timerfd_settime(timer_fd,TFD_TIMER_ABSTIME, &new_time, NULL))
	{
		LOG_ERR("timerfd_settime failed");
		goto EXIT;
	}

	FD_ZERO(&readset);
	FD_SET(timer_fd,&readset);
	for(total_time=0;;)
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = LOOP_TIMEOUT_MICROSECOND;

		//int select(int nfds,fd_set* readset,fd_set* writeset,fe_set* exceptset,struct timeval* timeout);
		select_res = select(max_fd+1,&readset,NULL,NULL,&timeout);
		if(select_res>0)
		{
			if(FD_ISSET(timer_fd,&readset))//file descriptor in set
			{
				LOG_DEBUG("selected fd is readable");
				if(size_64bits == read(timer_fd,&expire_time,sizeof(uint64_t)))
				{
					total_time += expire_time;
					LOG_DEBUG("total_time=%llu, expire_time=%llu",
						(unsigned long long)total_time,(unsigned long long)expire_time);
					continue;
				}
				LOG_ERR("Read timer_fd failed");
			}
			LOG_ERR("FD_ISSET = 0, ERROR!");
		}
		else if(0==select_res)
		{
			LOG_INFO("reach timeout value %ds %dus",timeout.tv_sec,timeout.tv_usec);
		}
		else
		{
			LOG_ERR("selected fd is not readable");
			goto EXIT;
		}
	}
	exit(OK);
EXIT:
	exit(ERROR);
}

void platform_event_loop()
{

}


