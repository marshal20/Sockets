#include "util.hpp"

namespace nt
{

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
			sockaddr_in* temp = (sockaddr_in*)sockaddr;

			address = create_address_from_ipv4addr(temp->sin_addr.s_addr);
			port = ntohs(temp->sin_port);
			return;
		}

		// IPv6
		sockaddr_in6* temp = (sockaddr_in6*)sockaddr;

		address = create_address_from_ipv6addr(&temp->sin6_addr);
		port = ntohs(temp->sin6_port);
	}

	// Works with value in network byte order.
	Address create_address_from_ipv4addr(uint32_t inaddr)
	{
		const unsigned char* vals = (const unsigned char*)&inaddr;

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

}
