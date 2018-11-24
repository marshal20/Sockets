#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

namespace socketimpl
{

	extern int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
	extern int socket(int af, int type, int protocol);

}
