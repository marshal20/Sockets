#include "util.hpp"

void create_sockaddr_from_address(const Address& address, unsigned short port, sockaddr_storage* sockaddr)
{
	if (address.m_addr.family == Family::IPv4)
	{
		sockaddr->ss_family = PF_INET;
		sockaddr_in* temp = (sockaddr_in*)sockaddr;
		unsigned char v4_interm[4] = { address.m_addr.v4.a, address.m_addr.v4.b, address.m_addr.v4.c, address.m_addr.v4.d };
		memset(&temp->sin_addr, 0, sizeof(sockaddr_in));
		memcpy(&temp->sin_addr, v4_interm, sizeof(v4_interm));
		temp->sin_port = htons(port);
		return;
	}

	// IPv6
	sockaddr->ss_family = PF_INET6;
	sockaddr_in6* temp = (sockaddr_in6*)sockaddr;
	unsigned short v6_interm[8] = { 
		htons(address.m_addr.v6.a), htons(address.m_addr.v6.b), htons(address.m_addr.v6.c), htons(address.m_addr.v6.d),
		htons(address.m_addr.v6.e), htons(address.m_addr.v6.f), htons(address.m_addr.v6.g), htons(address.m_addr.v6.h) 
	};
	memset(&temp->sin6_addr, 0, sizeof(sockaddr_in6));
	memcpy(&temp->sin6_addr, v6_interm, sizeof(v6_interm));
	temp->sin6_port = htons(port);
}

void create_address_from_sockaddr(Address& address, unsigned short& port, const sockaddr_storage* sockaddr)
{
	if (sockaddr->ss_family == PF_INET)
	{
		IPv4 v4;

		sockaddr_in* temp = (sockaddr_in*)sockaddr;
		unsigned char* v4_interm = (unsigned char*)&temp->sin_addr;
		v4.a = v4_interm[0]; v4.b = v4_interm[1];
		v4.c = v4_interm[2]; v4.d = v4_interm[3];

		address = v4;
		port = ntohs(temp->sin_port);
		return;
	}

	// IPv6
	IPv6 v6;

	sockaddr_in6* temp = (sockaddr_in6*)sockaddr;
	unsigned short* v6_interm = (unsigned short*)&temp->sin6_addr;
	
	v6.a = ntohs(v6_interm[0]); v6.b = ntohs(v6_interm[1]);
	v6.c = ntohs(v6_interm[2]); v6.d = ntohs(v6_interm[3]);
	v6.e = ntohs(v6_interm[4]); v6.f = ntohs(v6_interm[5]);
	v6.g = ntohs(v6_interm[6]); v6.h = ntohs(v6_interm[7]);

	address = v6;
	port = ntohs(temp->sin6_port);
}

// Works with ints in network byte order.
Address create_address_from_ipv4addr(int val)
{
	const unsigned char* vals = (const unsigned char*)&val;

	return IPv4{
		vals[0],
		vals[1],
		vals[2],
		vals[3]
	};
}

// Works with value in network byte order.
Address create_address_from_ipv6addr(const struct in6_addr* in6addr)
{
	const unsigned short* vals = (const unsigned short*)in6addr;

	return IPv6{
		ntohs(vals[0]),
		ntohs(vals[1]),
		ntohs(vals[2]),
		ntohs(vals[3]),
		ntohs(vals[4]),
		ntohs(vals[5]),
		ntohs(vals[6]),
		ntohs(vals[7])
	};
}
