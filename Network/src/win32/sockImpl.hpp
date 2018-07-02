#pragma once
#include <sys/types.h>
#include <winsock2.h>
#include <ws2def.h>
#include <ws2tcpip.h>

typedef SOCKET SocketHandle;

class sockImpl
{
public:
	sockImpl();
	~sockImpl();

	static int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints,	struct addrinfo **res);
	static SocketHandle socket(int af, int type, int protocol);

};