#include "Network/Address.hpp"

#include <stdio.h>
#include <string.h>
#include <exception>

Address::Address()
{
}

Address::~Address()
{
}

unsigned short Address::getPort() const
{
	// IPv4
	if (m_addr.ss_family == AF_INET)
	{
		return ntohs(((sockaddr_in*)&m_addr)->sin_port);
	}

	// IPv6
	return ntohs(((sockaddr_in6*)&m_addr)->sin6_port);
}

void Address::setPort(short value)
{
	// IPv4
	if (m_addr.ss_family == AF_INET)
	{
		((sockaddr_in*)&m_addr)->sin_port = htons(value);
		return;
	}

	// IPv6
	((sockaddr_in6*)&m_addr)->sin6_port = htons(value);
}

void Address::setIP(IPv4 value)
{
	m_addr.ss_family = AF_INET;
	((sockaddr_in*)&m_addr)->sin_addr.S_un.S_addr = htonl(value.val);
}

void Address::setIP(IPv6 value)
{
	m_addr.ss_family = AF_INET6;
	unsigned short conv[8] = { htons(value.a), htons(value.b),
								htons(value.c), htons(value.d),
								htons(value.e), htons(value.f),
								htons(value.g), htons(value.h) };
	memcpy(&(((sockaddr_in6*)&m_addr)->sin6_addr), conv, sizeof(conv));
}

std::string Address::getPresentation()
{
	static const int temp_len = INET6_ADDRSTRLEN;
	static char temp[temp_len + 1];
	void* addr;

	if (m_addr.ss_family == AF_INET)
	{
		struct sockaddr_in* ipv4 = (struct sockaddr_in*)&m_addr;
		addr = &(ipv4->sin_addr);
	}
	else
	{
		struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)&m_addr;
		addr = &(ipv6->sin6_addr);
	}
	
	inet_ntop(m_addr.ss_family, addr, temp, temp_len);

	temp[temp_len] = '\0';

	return std::string(temp);
}

std::vector<Address> Address::fromPresentationAll(const std::string& rep)
{
	std::vector<Address> addrs;

	struct addrinfo hints, *res, *it;
	const char* domainname = rep.c_str();

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int code = getaddrinfo(domainname, NULL, &hints, &res);
	if (code != 0)
	{
		std::string msg = "getaddrinfo() error: " + std::string(gai_strerrorA(code));
		throw std::exception(msg.c_str(), code);
	}

	for (it = res; it != NULL; it = it->ai_next)
	{
		Address temp;
		int struct_size = (it->ai_family == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
		memcpy(&temp.m_addr, it->ai_addr, struct_size);
		if (it->ai_family == PF_INET)
		{

		}
		else
		{

		}
		addrs.push_back(temp);
	}

	freeaddrinfo(res);

	return addrs;
}

Address Address::fromPresentation(const std::string& rep)
{
	return fromPresentationAll(rep)[0];
}

Address Address::localhost()
{
	// TODO
	return Address();
}

Address Address::broadcast()
{
	// TODO
	return Address();
}


#if WIN32
#include <ws2tcpip.h>
#include <ws2def.h>
#include <stdio.h>

// https://docs.microsoft.com/en-us/windows/desktop/WinSock/complete-client-code

struct WindowsSocket2Init
{
	WindowsSocket2Init()
	{
		printf("Initializing\n");
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR) {
			//wprintf(L"WSAStartup failed with error: %ld\n", iResult);
			//return 1;
			printf("WSAStartup failed with error: %ld\n", iResult);
		}
		printf("WSAStartup succeded\n");
	}

	~WindowsSocket2Init()
	{
		WSACleanup();
	}
};

WindowsSocket2Init global_ws2init;
#endif
