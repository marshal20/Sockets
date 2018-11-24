#include "unix/sockImpl.hpp"

namespace socketimpl
{

	int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
	{
		return ::getaddrinfo(node, service, hints, res);
	}

	int socket(int af, int type, int protocol)
	{
		return ::socket(af, type, protocol);
	}

}
