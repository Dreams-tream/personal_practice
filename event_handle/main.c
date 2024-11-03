#include<stdio.h>
#include"type.h"

#define LOG(fmt,args...) printf("[%s,%d]" fmt "\n", __func__, __LINE__, ##args)

int main()
{
	enum event_type event = EVENT_TCP;
	LOG("%s", enum_to_str(event));
	return 0;
}
