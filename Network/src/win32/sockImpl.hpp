#pragma once
#include <sys/types.h>
#include <winsock2.h>
#include <ws2def.h>
#include <ws2tcpip.h>

typedef int socklen_t;

int close(int sock);

class sockImpl
{
public:
	sockImpl();
	~sockImpl();

	static int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints,	struct addrinfo **res);
	static int socket(int af, int type, int protocol);

};