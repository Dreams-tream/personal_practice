/* for ioctl */
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ether.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <net/route.h>

/* DIR *opendir(const char *name); */
#include <dirent.h>
//#include <sys/types.h>

#include "log.h"
#include "module_ioctl.h"


/***************************************MACRO*******************************************/
#define NET_DIR                                "/sys/class/net/"


/*************************************FUNCTIONS*****************************************/
STATUS ioctl_get_ip_mac_mask_address(char ipaddr[], int len)
{
	int socketfd;
	STATUS ret = ERROR;
	struct ifreq struReq;
	unsigned char ipaddress[INET_MACADDRESSLEN];
	unsigned char macaddress[INET_MACADDRESSLEN];
	unsigned char netmask[INET_MACADDRESSLEN];

	if (!ipaddr || len < 0)
	{
		return ERROR;
	}

	memset(&struReq, 0x00, sizeof(struct ifreq));
	strncpy(struReq.ifr_name, "eth0", sizeof(struReq.ifr_name));

	socketfd = socket(PF_INET, SOCK_STREAM, 0);

	if (-1 == ioctl(socketfd, SIOCGIFHWADDR, &struReq))
	{
		LOG_ERR("ioctl hwaddr error!\n");
		goto END;
	}

	strcpy((char *)macaddress, ether_ntoa((ether_addr*)struReq.ifr_hwaddr.sa_data));

	if (-1 == ioctl(socketfd, SIOCGIFADDR, &struReq))
	{
		LOG_ERR("ioctl ip address error!\n");
		goto END;
	}

	strcpy((char *)ipaddress, inet_ntoa(((struct sockaddr_in *)&(struReq.ifr_addr))->sin_addr));

	if (-1 == ioctl(socketfd, SIOCGIFNETMASK, &struReq))
	{
		LOG_ERR("ioctl net mask error!\n");
		goto END;
	}

	ret = OK;
	strcpy((char *)netmask, inet_ntoa(((struct sockaddr_in *)&(struReq.ifr_netmask))->sin_addr));

	LOG_ERR("MacAddress: %s\n", macaddress);
	LOG_ERR("IpAddress: %s\n", ipaddress);
	LOG_ERR("NetMask: %s\n",netmask);


END:
	if (socketfd > -1)
	{
		close(socketfd);
	}

	return ret;
}

