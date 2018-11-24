#pragma once
#include <sys/types.h>
#include <WinSock2.h>
//#include <ws2def.h>
#include <ws2tcpip.h>

typedef int socklen_t;

extern int close(int sock);

namespace socketimpl
{
	extern int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
	extern int socket(int af, int type, int protocol);
}
