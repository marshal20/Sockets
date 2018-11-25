#pragma once
#include <sys/types.h>
#include <WinSock2.h>
//#include <ws2def.h>
#include <ws2tcpip.h>

#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH

typedef int socklen_t;

extern int close(int sock);

namespace socketimpl
{
	extern int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
	extern int socket(int af, int type, int protocol);
}
