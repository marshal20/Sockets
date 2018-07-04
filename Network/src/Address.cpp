#include "Network/Address.hpp"
#include "errors.hpp"
#include "sockImpl.hpp"

#if WIN32
#include <ws2tcpip.h>
#include <ws2def.h>
#else if __linux__
#include <sys/types.h>
#include <sys/socket.h>
#endif

void addrinfoTosockaddrstorage(const addrinfo* src, sockaddr_storage* dst)
{
	memset(dst, 0, sizeof(sockaddr_storage));
	int struct_size = (src->ai_family == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
	memcpy(dst, src->ai_addr, struct_size);
}

Address::Address()
{

}

Address::~Address()
{
}

unsigned short Address::getPort() const
{
	if (!m_valid) return 0;

	// IPv4
	if (m_addr.ss_family == AF_INET)
	{
		return ntohs(((sockaddr_in*)&m_addr)->sin_port);
	}

	// IPv6
	return ntohs(((sockaddr_in6*)&m_addr)->sin6_port);
}

Address Address::setPort(short value)
{
	m_valid = true;
	// IPv4
	if (m_addr.ss_family == AF_INET)
	{
		((sockaddr_in*)&m_addr)->sin_port = htons(value);
		return *this;
	}

	// IPv6
	((sockaddr_in6*)&m_addr)->sin6_port = htons(value);
	return *this;
}

Address Address::setIP(const IPv4& value)
{
	m_valid = true;
	m_addr.ss_family = AF_INET;
	((sockaddr_in*)&m_addr)->sin_addr.s_addr = htonl(value.val);
}

Address Address::setIP(const IPv6& value)
{
m_valid = true;
m_addr.ss_family = AF_INET6;
unsigned short conv[8] = { htons(value.a), htons(value.b),
							htons(value.c), htons(value.d),
							htons(value.e), htons(value.f),
							htons(value.g), htons(value.h) };
memcpy(&(((sockaddr_in6*)&m_addr)->sin6_addr), conv, sizeof(conv));
}

Protocol Address::getProtocol() const
{
	// IPv4
	if (m_addr.ss_family == PF_INET) return Protocol::IPv4;

	// IPv6
	return Protocol::IPv6;
}

std::string Address::getPresentation() const
{
	if (!m_valid) return "Invalid address";

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

	int code = sockImpl::getaddrinfo(domainname, NULL, &hints, &res);
	if (code != 0)
		Error::runtime("getaddrinfo failed", gai_strerror(code), code);

	for (it = res; it != NULL; it = it->ai_next)
	{
		Address temp;
		addrinfoTosockaddrstorage(it, &temp.m_addr);
		temp.m_valid = true;
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
	Address temp;
	struct addrinfo hint, *res;
	int code;
	memset(&hint, 0, sizeof(struct addrinfo));
	hint.ai_family = AF_INET;

	if ((code = sockImpl::getaddrinfo("localhost", NULL, &hint, &res)) != 0)
		Error::runtime("getaddrinfo failed", gai_strerror(code), code);

	temp.m_valid = true;
	addrinfoTosockaddrstorage(res, &temp.m_addr);
	freeaddrinfo(res);

	return temp;
}

Address Address::broadcast()
{
	// TODO
	return Address();
}

void AddressTosockaddr(const Address& val, sockaddr_storage* sockaddr)
{
	if (val.m_addr.type == Protocol::IPv4)
	{
		sockaddr->ss_family = PF_INET;
		sockaddr_in* temp = (sockaddr_in*)sockaddr;
		unsigned char v4_interm[4] = { val.m_addr.v4.a, val.m_addr.v4.b, val.m_addr.v4.c, val.m_addr.v4.d };
		memcpy(&temp->sin_addr, v4_interm, sizeof(v4_interm));
		temp->sin_port = htons(val.m_addr.port);
		return;
	}

	// IPv6
	sockaddr->ss_family = PF_INET6;
	sockaddr_in6* temp = (sockaddr_in6*)sockaddr;
	unsigned char v6_interm[16] = { val.m_addr.v6.a, val.m_addr.v6.b, val.m_addr.v6.c,val.m_addr.v6.d,
		val.m_addr.v6.e,val.m_addr.v6.f,val.m_addr.v6.g,val.m_addr.v6.h };
	memcpy(&temp->sin6_addr, v6_interm, sizeof(v6_interm));
	temp->sin6_port = htons(val.m_addr.port);
}

void sockaddrToAddress(Address& val, const sockaddr_storage* sockaddr)
{
	if (sockaddr->ss_family == PF_INET)
	{

		return;
	}

	// IPv6
}


Address Address::setIP(const IPv4& value)
{
	m_valid = true;
	m_addr.ss_family = AF_INET;
	((sockaddr_in*)&m_addr)->sin_addr.s_addr = htonl(value.val);
}

Address Address::setIP(const IPv6& value)
{
	m_valid = true;
	m_addr.ss_family = AF_INET6;
	unsigned short conv[8] = { htons(value.a), htons(value.b),
		htons(value.c), htons(value.d),
		htons(value.e), htons(value.f),
		htons(value.g), htons(value.h) };
	memcpy(&(((sockaddr_in6*)&m_addr)->sin6_addr), conv, sizeof(conv));
}