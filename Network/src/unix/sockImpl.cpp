#include "unix/sockImpl.hpp"

sockImpl::sockImpl()
{

}

sockImpl::~sockImpl()
{

}

int sockImpl::getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
	return ::getaddrinfo(node, service, hints, res);
}

SocketHandle sockImpl::socket(int af, int type, int protocol)
{
	return ::socket(af, type, protocol);
}