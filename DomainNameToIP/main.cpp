#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("- Usage ./socketstest <domain name>\n");
		return 1;
	}

	struct addrinfo hints, *res, *it;
	char addrstr[INET6_ADDRSTRLEN];
	char* domainname = argv[1];
	
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int code = getaddrinfo(domainname, NULL, &hints, &res);
	if(code != 0)
	{
		printf("- Error: %s.\n", gai_strerror(code));
		return 1;
	}
	
	printf("- Addresses for %s:\n", domainname);
	
	// iterating over the linked list
	for(it = res; it != NULL; it = it->ai_next)
	{
		void* addr;
		const char* ver;

		if(it->ai_family == AF_INET)
		{
			struct sockaddr_in* ipv4 = (struct sockaddr_in*)it->ai_addr;
			addr = &(ipv4->sin_addr);
			ver = "IPv4";
		}
		else
		{
			struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)it->ai_addr;
			addr = &(ipv6->sin6_addr);
			ver = "IPv6";
		}
		
		inet_ntop(it->ai_family, addr, addrstr, sizeof(addrstr));
		printf("%s: %s\n", ver, addrstr);
	}
	
	freeaddrinfo(res);
	return 0;
}
