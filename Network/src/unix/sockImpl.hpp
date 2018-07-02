#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

typedef int SocketHandle;

class sockImpl
{
public:
	sockImpl();
	~sockImpl();

	static int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
	static SocketHandle socket(int af, int type, int protocol);

};