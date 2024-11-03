#include<stdio.h>
#include"log.h"

int main()
{
	LOG("you are a foolish");
	CMD("sleep 5");
	LOG("MMM:%s", STR(MMM));
	return 0;
}
