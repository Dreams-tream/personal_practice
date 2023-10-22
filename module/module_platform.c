#include"module_platform.h"
#include"module.h"
#include"log.h"
#include<stdio.h>
#include<json.h>

extern module_cfg g_module_cfg;
extern int dbg_level;

void_func(PRINT_MODULE_CONFIG)
{
	LOG_DEBUG("author:%s",g_module_cfg.conf.author);
	LOG_DEBUG("seconds:%u",g_module_cfg.conf.seconds);
}

int module_load_config()
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

void module_init_platform()
{

}

void module_timer_loop()
{

}

void module_event_loop()
{

}


