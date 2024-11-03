#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <netinet/in.h>

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
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[MAX_CMD_LEN];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        s = strcmp(ifa->ifa_name, "eth0");
        if (s != 0)
            continue;

        if (family == AF_INET) { // IPv4
            struct sockaddr_in *in_addr = (struct sockaddr_in *)ifa->ifa_addr;
            inet_ntop(family, &(in_addr->sin_addr), host, MAX_CMD_LEN);
            strncpy(ipaddr, host, len - 1);
            break;
        } else if (family == AF_INET6) { // IPv6
            struct sockaddr_in6 *in6_addr = (struct sockaddr_in6 *)ifa->ifa_addr;
            inet_ntop(family, &(in6_addr->sin6_addr), host, MAX_CMD_LEN);
            strncpy(ipaddr, host, len - 1);
            break;
        }
    }

    freeifaddrs(ifaddr);
	return OK;
}

