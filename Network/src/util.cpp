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
		address.m_addr.family = Family::IPv4;
		sockaddr_in* temp = (sockaddr_in*)sockaddr;
		unsigned char* v4_interm = (unsigned char*)&temp->sin_addr;
		address.m_addr.v4.a = v4_interm[0]; address.m_addr.v4.b = v4_interm[1];
		address.m_addr.v4.c = v4_interm[2]; address.m_addr.v4.d = v4_interm[3];
		port = ntohs(temp->sin_port);
		return;
	}

	// IPv6
	address.m_addr.family = Family::IPv6;
	sockaddr_in6* temp = (sockaddr_in6*)sockaddr;
	unsigned short* v6_interm = (unsigned short*)&temp->sin6_addr;
	// make an array of unsigned short so that we don't worry about endians
	address.m_addr.v6.a = ntohs(v6_interm[0]); address.m_addr.v6.b = ntohs(v6_interm[1]);
	address.m_addr.v6.c = ntohs(v6_interm[2]); address.m_addr.v6.d = ntohs(v6_interm[3]);
	address.m_addr.v6.e = ntohs(v6_interm[4]); address.m_addr.v6.f = ntohs(v6_interm[5]);
	address.m_addr.v6.g = ntohs(v6_interm[6]); address.m_addr.v6.h = ntohs(v6_interm[7]);

	port = ntohs(temp->sin6_port);
}

Address create_address_from_ipv4addr(int val)
{
	Address temp;
	temp.m_valid = true;
	*(int*)&temp.m_addr.v4 = ntohl(val);
	temp.m_addr.family = Family::IPv4;
	return temp;
}

Address create_address_from_ipv6addr(const struct in6_addr* in6addr)
{
	Address temp;
	unsigned short dummy_port = 0;
	temp.m_valid = true;
	sockaddr_in6 temp_addr6;
	memset(&temp_addr6, 0, sizeof(temp_addr6));
	temp_addr6.sin6_family = PF_INET6;
	memcpy(&temp_addr6.sin6_addr, in6addr, sizeof(in6_addr));
	create_address_from_sockaddr(temp, dummy_port, (const sockaddr_storage*)&temp_addr6);
	return temp;
}
