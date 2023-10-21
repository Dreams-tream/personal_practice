#include"module_platform.h"
#include"module.h"
#include"log.h"
#include<stdio.h>

extern module_cfg g_module_cfg;
extern int dbg_level;

int get_config_from_json(const char *conf_file, module_config *conf)
{
	return OK;
}
int module_load_config()
{
	FILE* fp = NULL;
	char conf_file[MODULE_FILE_LEN+1] = {0};
	char empty_author[AUTHOR_NAME_LEN+1] = {0};
	char author[AUTHOR_NAME_LEN+1] = {0};

	if(memcmp(g_module_cfg.conf.author,author,AUTHOR_NAME_LEN))
	{
		memmove(author,g_module_cfg.conf.author,AUTHOR_NAME_LEN);
		LOG_ERR("No config file! Use default");
	}
	else
		snprintf(author,AUTHOR_NAME_LEN,"%s",DEFAULT_AUTHOR_NAME);

	snprintf(conf_file,MODULE_FILE_LEN,"%s%s%s",MODULE_CODE_DIR,author,CONFIG_FILE_POSTFIX);
	fp = fopen(conf_file,"w+");
	if(!fp)
	{
		LOG_ERR("open %s fail!",conf_file);
		return ERROR;
	}

	if(ERROR==get_config_from_json(conf_file,&g_module_cfg.conf))
		return ERROR;

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


