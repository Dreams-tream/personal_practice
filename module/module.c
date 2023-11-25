#include "module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>                  /*Definition of getopt()*/
#include <json_object.h>
#include<json_util.h>
#include"log.h"

#define MAX_ERROR_TIMES                    5

static module_cfg g_module_cfg;
extern char g_virtule_console[MAX_DEVICE_LEN];

int module_parse_parameter(int argc,char **argv,const char *optstring);
void_func(USAGE);
void_func(monitor_signal);
void signal_process(int sig);
int_func(create_config_json_file);

#ifdef CONFIG_AUTHOR_NAME_SUPPORT
static void get_author_name_from_config(char *author);
#endif

void_func(PRINT_MODULE_CONFIG);
int_func(module_load_config);
int_func(create_pid_file);
int_func(get_current_virtul_console);

int module_parse_parameter(int argc,char **argv,const char *optstring)
{
	int opt;
	int log_level;

	while((opt = getopt(argc,argv,optstring)) != ERROR)
	{
		switch(opt)
		{
		case 'a':
			if(strlen(optarg))
			{
				LOG_DEBUG("author is %s",optarg);
				memmove(g_module_cfg.conf.author,optarg,AUTHOR_NAME_LEN);
				str_replace(g_module_cfg.conf.author,' ','_');
			}else
			{
				LOG_ERR("invalid length of author");
				goto HELP;
			}
			break;
		case 's':
			if(strspn(optarg, "0123456789") == strlen(optarg))
			{
				LOG_DEBUG("second is %s",optarg);
				g_module_cfg.conf.second=atoi(optarg);
			}else
			{
				LOG_ERR("%s: invalid number",optarg);
				goto HELP;
			}
			break;
		case 'm':
			if(strspn(optarg, "0123456789") == strlen(optarg))
			{
				LOG_DEBUG("millisecond is %s",optarg);
				g_module_cfg.conf.millisecond = atoi(optarg);
			}else
			{
				LOG_ERR("%s: invalid number",optarg);
				goto HELP;
			}
			break;
		case 'l':
			if(strspn(optarg, "0123456789") == strlen(optarg))
			{
				log_level = atoi(optarg);
				LOG_DEBUG("log level is %s",LogLevelToStr(log_level));
				modify_log_level(0,log_level);
			}else
			{
				LOG_ERR("%s: please input right log level",optarg);
				goto HELP;
			}
			break;
HELP:
		case 'h':
		default:
			if(':'==opt || !strchr(optstring,opt))
				LOG_ERR("unkonwn option: '%c'",opt);
			USAGE();
			exit(-1);
		}
	}

	return OK;
}

void USAGE()
{
	int log_level = 0;
	PRINT("======================================================================================");
	PRINT("Help Information:");
	PRINT("    -a [author]        input author's name");
	PRINT("    -h                 print help information");
	PRINT("    -s [second]        input second to determine timer period of unit second");
	PRINT("    -l [log_level]     change log_level to increase/decrease debug information");
	for(log_level=0;log_level<LOG_LEVEL_MAX;log_level++)
		PRINT("                       %d: %s",log_level,LogLevelToStr(log_level));
	PRINT("    -m [millisecond]   input millisecond to determine timer period of unit millisecond");
	PRINT("======================================================================================");
}

void monitor_signal()
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
	case SIGUSR2:
		modify_log_level(sig,0);
		break;
	default:
		LOG_ERR("Unknown signal, break");
		break;
	}
}

#ifdef CONFIG_AUTHOR_NAME_SUPPORT
static void get_author_name_from_config(char *author)
{
	int len;
	FILE *fp = NULL;
	char conf_file[MODULE_FILE_LEN+1] = "MODULE.cfg";

	if(strlen(g_module_cfg.conf.author) || !author)
		return;

	if(NULL == (fp=fopen(conf_file,"r")))
		LOG_ERR("open %s failed",conf_file);

	fread(author,1,AUTHOR_NAME_LEN,fp);
	len = strlen(author);
	if(len && '\n' == author[len-1])
	{
		author[len-1] = '\0';
		len--;
	}
	str_replace(author,' ','_');

	if(len)
		memmove(&g_module_cfg.conf.author,author,AUTHOR_NAME_LEN);
	else
		memmove(author,DEFAULT_AUTHOR_NAME,AUTHOR_NAME_LEN);
	PRINT_MODULE_CONFIG();
	fclose(fp);
}
#endif

int create_config_json_file()
{
	int ret = OK;
	int second = g_module_cfg.conf.second;
	int millisecond = g_module_cfg.conf.millisecond;
	char author[AUTHOR_NAME_LEN+1] = {0};
	char conf_file[MODULE_FILE_LEN] = {0};
	json_object *j_obj,*j_str,*j_int;

	j_obj=j_str=j_int=NULL;
	if(!strlen(g_module_cfg.conf.author))
	{
#ifndef CONFIG_AUTHOR_NAME_SUPPORT
		memmove(author,DEFAULT_AUTHOR_NAME,AUTHOR_NAME_LEN);
		str_replace(author,' ','_');
		LOG_ERR("No author argument input! Use default author!");
#else
		get_author_name_from_config(author);
#endif
	}else
		memmove(author,g_module_cfg.conf.author,AUTHOR_NAME_LEN);

	snprintf(conf_file,MODULE_FILE_LEN-1,"%s%s%s",MODULE_CODE_DIR,author,CONFIG_FILE_POSTFIX);
	j_obj = json_object_new_object();//json_object_put
	if(NULL==j_obj)
	{
		ret = ERROR;
		goto END;
	}
	json_object_object_add(j_obj,"author",json_object_new_string(author));
	json_object_object_add(j_obj,"second",json_object_new_int(second>0?second:LOOP_TIMEOUT_SECOND));
	json_object_object_add(j_obj,"millisecond",json_object_new_int(millisecond>0?millisecond:LOOP_TIMEOUT_MILLISECOND));
	LOG_ERR("%s:%s",conf_file,json_object_to_json_string(j_obj));
END:
	if(j_obj){
		json_object_to_file(conf_file,j_obj);
		json_object_put(j_obj);
		j_obj = NULL;
	}
	return ret;
}

void PRINT_MODULE_CONFIG()
{
	LOG_ERR("author:%s",g_module_cfg.conf.author);
	LOG_ERR("second:%us",g_module_cfg.conf.second);
	LOG_ERR("millisecond:%ums",g_module_cfg.conf.millisecond);
}

int module_load_config()
{
	int second = 0;
	int millisecond = 0;
	char conf_file[MODULE_FILE_LEN+1] = {0};
	char author[AUTHOR_NAME_LEN+1] = {0};
	const char *p_author = NULL;
	json_object *j_obj,*j_tmp;

	if(strlen(g_module_cfg.conf.author)>0)
	{
		memmove(author,g_module_cfg.conf.author,AUTHOR_NAME_LEN);
	}
	else
	{
		snprintf(author,AUTHOR_NAME_LEN,"%s",DEFAULT_AUTHOR_NAME);
	}

	str_replace(author,' ','_');
	snprintf(conf_file,MODULE_FILE_LEN,"%s%s%s",MODULE_CODE_DIR,author,CONFIG_FILE_POSTFIX);

	j_obj = j_tmp = NULL;
	j_obj = json_object_from_file(conf_file);//json_object_put
	if(NULL==j_obj)
	{
		LOG_ERR("Read json from config file fail");
		return ERROR;
	}

	j_tmp = json_object_object_get(j_obj,"author");
	p_author = json_object_get_string(j_tmp);
	j_tmp = json_object_object_get(j_obj,"second");
	second = json_object_get_int(j_tmp);
	j_tmp = json_object_object_get(j_obj,"millisecond");
	millisecond = json_object_get_int(j_tmp);

	if(strlen(g_module_cfg.conf.author)<=0)
		memmove(&g_module_cfg.conf.author,p_author,strlen(p_author));
	g_module_cfg.conf.second = second;
	g_module_cfg.conf.millisecond = millisecond;
	PRINT_MODULE_CONFIG();
	json_object_put(j_obj);
	return OK;
}

int create_pid_file()
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

int get_current_virtul_console()
{
	int len = 0;
	int err_cnt = 0;
	int need_repeat = 0;
	FILE *fp;
	char cmd[MAX_CMD_LEN+1]={0};
	char res[MAX_CMD_LEN+1]={0};

	snprintf(cmd,MAX_CMD_LEN,"w|grep -Ew 'w|%s' | awk '{print $2}'",MODULE_NAME);
	do{
		if((fp= popen(cmd,"r")))
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

	if(ERROR==module_parse_parameter(argc,argv,"a:s:m:l:h"))
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
