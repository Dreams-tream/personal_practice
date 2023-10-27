#include"module.h"
#include<stdio.h>
#include<signal.h>
#include <sys/types.h>
#include <unistd.h>                  /*Definition of getopt()*/
#include<json_object.h>
#include"log.h"

#define DEFAULT_PERIOD                     0/*s*/
#define MAX_ERROR_TIMES                    5


static module_cfg g_module_cfg;
extern int dbg_level;
extern char g_virtule_console[MAX_DEVICE_LEN];

int module_parse_parameter(int argc,char **argv,const char *optstring);
void_func(USAGE);
void_func(monitor_signal);
void signal_process(int sig);
int_func(create_config_json_file);
void_func(PRINT_MODULE_CONFIG);
int_func(module_load_config);
int_func(create_pid_file);
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
			str_replace(g_module_cfg.conf.author,' ','_');
			mask |= 0x01;
			break;
		case 'p':
			PRINT("period is %s",optarg);
			g_module_cfg.conf.period=atoi(optarg);
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
		module_exit(&g_module_cfg);
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

int_func(create_config_json_file)
{
	int ret = OK;
	int period = g_module_cfg.conf.period;
	char *author = g_module_cfg.conf.author;
	char empty_author[AUTHOR_NAME_LEN+1] = {0};
	char conf_file[MODULE_FILE_LEN] = {0};
	json_object *j_obj,*j_str,*j_int;

	j_obj=j_str=j_int=NULL;
	if(!memcmp(author,empty_author,AUTHOR_NAME_LEN))
	{
		LOG_ERR("Author is empty! Use default!");
		author = DEFAULT_AUTHOR_NAME;
	}
	if(strlen(author)<=0 || strlen(author)>AUTHOR_NAME_LEN)
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
	json_object_object_add(j_obj,"period",json_object_new_int(period>0?period:DEFAULT_PERIOD));
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
	LOG_DEBUG("period:%u%s",g_module_cfg.conf.period,"s");
}

int_func(module_load_config)
{
	FILE* fp = NULL;
	int period = 0;
	char conf_file[MODULE_FILE_LEN+1] = {0};
	char author[AUTHOR_NAME_LEN+1] = {0};
	char *p_author = NULL;
	json_object *j_obj,*j_tmp;


	if(strlen(g_module_cfg.conf.author)>0)
	{
		memmove(author,g_module_cfg.conf.author,AUTHOR_NAME_LEN);
	}
	else
	{
		snprintf(author,AUTHOR_NAME_LEN,"%s",DEFAULT_AUTHOR_NAME);
		LOG_ERR("No config file! Use default");
	}

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
	j_tmp = json_object_object_get(j_obj,"period");
	period = json_object_get_int(j_tmp);

	if(strlen(g_module_cfg.conf.author)<=0)
		memmove(&g_module_cfg.conf.author,p_author,strlen(p_author));
	g_module_cfg.conf.period = period;
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
	LOG_ERR("pid_file=%s, pid= %d",pid_file, pid);
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
	int len = 0;
	int err_cnt = 0;
	int need_repeat = 0;
	FILE *fp;
	char res[MAX_CMD_LEN+1]={0};

	do{
		if(fp= popen("w|grep -w w| awk '{print $2}'","r"))
		{
			fgets(res,MAX_CMD_LEN,fp);
			pclose(fp);

			len=strlen(res);
			if(len<=0)
			{
				printf("[%s,%d]len=%d, need repeat\n",__FUNCTION__,__LINE__,len);
				goto REPEAT;
			}

			while(len>0 && ('\n'==res[len-1] || '\t'==res[len-1] || ' '==res[len-1]))
				res[(len--)-1] = '\0';

			if(strlen(res))
			{
				memmove(g_virtule_console,res,strlen(res));
				printf("[%s,%d]current virtual console is '%s%s'\n",__FUNCTION__,__LINE__,"/dev/",g_virtule_console);
				return OK;
			}
		}
		else
		{
			printf("[%s,%d]popen failed, need repeat\n",__FUNCTION__,__LINE__);
		}
REPEAT:
		memset(res,0,sizeof(res));
		err_cnt++;
		need_repeat = 1;
	}while(need_repeat && err_cnt<MAX_ERROR_TIMES);
	return ERROR;
}

int main(int argc, char **argv)
{
	memset(&g_module_cfg,0,sizeof(g_module_cfg));
	memset(g_virtule_console,0,sizeof(g_virtule_console));

	if(ERROR==get_current_virtul_console())
	{
		perror("cannot get current console!");
		return ERROR;
	}

	if(ERROR==module_parse_parameter(argc,argv,"a:p:h"))
	{
		LOG_ERR("parse parameter failed!");
		goto EXIT;
	}

	if(ERROR==create_config_json_file())
	{
		LOG_ERR("Create config file error!");
		goto EXIT;
	}

	if(ERROR==module_load_config())
	{
		LOG_ERR("load config file error!");
		goto EXIT;
	}

	if(ERROR==create_pid_file())
	{
		LOG_ERR("Create pid file error!");
		goto EXIT;
	}

	monitor_signal();
	module_register_callbacks(&g_module_cfg.cb);
	module_init_platform(&g_module_cfg.cb);
	module_timer_loop(&g_module_cfg);
EXIT:
	module_exit(&g_module_cfg);
	return 0;
}
