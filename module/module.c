#include"module.h"
#include<stdio.h>
#include<signal.h>
#include <sys/types.h>
#include <unistd.h>                  /*Definition of getopt()*/
#include<json_object.h>
#include"log.h"

#define DEFAULT_TIME                       1000/*s*/

static module_cfg g_module_cfg;
extern int dbg_level;
extern char g_virtule_console[MAX_DEVICE_LEN];

int module_parse_parameter(int argc,char **argv,const char *optstring);
void_func(USAGE);
void_func(monitor_signal);
void signal_process(int sig);
int_func(create_pid_file);
void_func(PRINT_MODULE_CONFIG);
int_func(module_load_config);
int_func(get_current_virtul_console);

int module_parse_parameter(int argc,char **argv,const char *optstring)
{
	int opt;
	int ret = OK;
	int mask = 0;

	while((opt = getopt(argc,argv,optstring)) != ERROR)
	{
		switch(opt)
		{
		case 'a':
			PRINT("author is %s",optarg);
			memmove(g_module_cfg.conf.author,optarg,AUTHOR_NAME_LEN);
			mask |= 0x01;
			break;
		case 's':
			PRINT("seconds are %s",optarg);
			g_module_cfg.conf.seconds=atoi(optarg);
			mask |= 0x10;
		case 'h':
			USAGE();
			break;
		default:
			PRINT("unkonwn command!");
			ret = ERROR;
			break;
		}
	}

	if(mask & 0x11)
		PRINT_MODULE_CONFIG();
	return ret;
}

void_func(USAGE)
{

}

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
		module_exec_cmd("rm -f %s%s.*",MODULE_CODE_DIR,DEFAULT_AUTHOR_NAME);
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

void_func(PRINT_MODULE_CONFIG)
{
	LOG_DEBUG("author:%s",g_module_cfg.conf.author);
	LOG_DEBUG("seconds:%u",g_module_cfg.conf.seconds);
}

int_func(module_load_config)
{
	FILE* fp = NULL;
	int seconds = 0;
	char conf_file[MODULE_FILE_LEN+1] = {0};
	char author[AUTHOR_NAME_LEN+1] = {0};
	char *p_author = NULL;
	json_object *j_obj,*j_tmp;


	if(strlen(g_module_cfg.conf.author)>0)
	{
		memmove(author,g_module_cfg.conf.author,AUTHOR_NAME_LEN);
		LOG_ERR("No config file! Use default");
	}
	else
		snprintf(author,AUTHOR_NAME_LEN,"%s",DEFAULT_AUTHOR_NAME);

	snprintf(conf_file,MODULE_FILE_LEN,"%s%s%s",MODULE_CODE_DIR,author,CONFIG_FILE_POSTFIX);

	j_obj = j_tmp = NULL;
	j_obj = json_object_from_file(conf_file);
	if(NULL==j_obj)
	{
		LOG_ERR("Read json from config file fail");
		return ERROR;
	}

	j_tmp = json_object_object_get(j_obj,"author");
	p_author = json_object_get_string(j_tmp);
	j_tmp = json_object_object_get(j_obj,"seconds");
	seconds = json_object_get_int(j_tmp);

	memmove(&g_module_cfg.conf.author,p_author,strlen(p_author));
	g_module_cfg.conf.seconds = seconds;
	PRINT_MODULE_CONFIG();
	return OK;
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
	LOG_WARN("pid_file=%s, pid=%d",pid_file, pid);
	fp = fopen(pid_file, "w+");
	if(!fp)
	{
		return ERROR;
	}

	fprintf(fp,"%u\n",pid);
	fclose(fp);
	return OK;
}

int_func(get_current_virtul_console)
{
	int len=0;
	char res[MAX_CMD_LEN+1]={0};

	memset(g_virtule_console,0,MAX_DEVICE_LEN);
	if((-1) != module_exec_get_res("w|grep -w w| awk '{print $2}'",res))
	{
		len=strlen(res);
		if(len)
		{
			memmove(g_virtule_console,res,len);
			LOG_ERR("current virtual console is %s%s","/dev/",g_virtule_console);
			return OK;
		}
	}
	return ERROR;
}

int main(int argc, char **argv)
{
	char author[AUTHOR_NAME_LEN+1] = {0};
	unsigned int seconds = DEFAULT_TIME;

	if(ERROR==get_current_virtul_console())
	{
		perror("cannot get current console!");
		return ERROR;
	}

	if(ERROR==module_parse_parameter(argc,argv,"a:s:h"))
	{
		LOG_ERR("parse parameter failed!");
		return ERROR;
	}

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
	module_init_platform(&g_module_cfg.cb);
	module_timer_loop(&g_module_cfg.cb);
	return 0;
}
