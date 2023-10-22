#include<stdio.h>
#include<signal.h>
#include <sys/types.h>
#include <unistd.h>
#include<json_object.h>

#include"module.h"
#include"log.h"

#define DEFAULT_TIME                       1000/*s*/

module_cfg g_module_cfg;
extern int dbg_level;


void_func(monitor_signal);
void signal_process(int sig);
int_func(create_pid_file);

void_func(monitor_signal)
{
	signal(SIGINT,  signal_process);
	signal(SIGTERM, signal_process);
	signal(SIGUSR1, signal_process);
	signal(SIGUSR2, signal_process);
}

void signal_process(int sig)
{
	switch(sig)
	{
	case SIGTERM:
	case SIGINT:
		LOG_ERR("Exit this module");
		module_exec_cmd("rm -f %s%s%s",MODULE_CODE_DIR,DEFAULT_AUTHOR_NAME,CONFIG_FILE_POSTFIX);
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

int create_config_json_file(char *author,unsigned int seconds)
{
	int ret = OK;
	char empty_author[AUTHOR_NAME_LEN+1] = {0};
	char conf_file[MODULE_FILE_LEN] = {0};
	json_object *j_obj,*j_str,*j_int;

	j_obj=j_str=j_int=NULL;
	LOG_ERR("");
	if(!memcmp(author,empty_author,AUTHOR_NAME_LEN))
	{
		LOG_ERR("Author is empty! Use default!");
		memmove(author,DEFAULT_AUTHOR_NAME,strlen(DEFAULT_AUTHOR_NAME));
	}
	if(strlen(author)>AUTHOR_NAME_LEN)
	{
		LOG_ERR("Invalid author length!");
		return ERROR;
	}

	snprintf(conf_file,MODULE_FILE_LEN-1,"%s%s%s",MODULE_CODE_DIR,author,CONFIG_FILE_POSTFIX);
	j_obj = json_object_new_object();
	if(NULL==j_obj)
	{
		ret = ERROR;
		goto END;
	}
	json_object_object_add(j_obj,"author",json_object_new_string(author));
	json_object_object_add(j_obj,"seconds",json_object_new_int(seconds>0?seconds:DEFAULT_TIME));
	LOG_ERR("%s:%s",conf_file,json_object_to_json_string(j_obj));
END:
	if(j_obj){
		json_object_to_file(conf_file,j_obj);
		json_object_put(j_obj);
		j_obj = NULL;
	}
	return ret;
}

int_func(create_pid_file)
{
	FILE *fp = NULL;
	pid_t pid = getpid();
	char pid_file[MODULE_FILE_LEN+1] = {0};
	char author[AUTHOR_NAME_LEN+1] = {0};

	strlen(g_module_cfg.conf.author)<=0?memmove(author,DEFAULT_AUTHOR_NAME,strlen(DEFAULT_AUTHOR_NAME)):\
		memmove(author,g_module_cfg.conf.author,AUTHOR_NAME_LEN);
	snprintf(pid_file,MODULE_FILE_LEN,"%s%s.pid",MODULE_CODE_DIR,author);
	LOG_WARN("pid_file=%s",pid_file);
	fp = fopen(pid_file, "w+");
	if(!fp)
	{
		return ERROR;
	}

	fprintf(fp,"%u\n",pid);
	fclose(fp);
	return OK;
}

int main(int argc, char **argv)
{
	char author[AUTHOR_NAME_LEN+1] = {0};
	unsigned int seconds = DEFAULT_TIME;

	monitor_signal();
	if(ERROR==create_config_json_file(author,seconds))
	{
		LOG_ERR("Create config file error!");
		return ERROR;
	}

	memset(&g_module_cfg,0,sizeof(g_module_cfg));
	module_load_config();

	if(ERROR==create_pid_file())
	{
		LOG_ERR("Create pid file error!");
		return ERROR;
	}

	module_register_callbacks(&g_module_cfg.cb);
	module_init_platform();
	module_timer_loop();
	return 0;
}
