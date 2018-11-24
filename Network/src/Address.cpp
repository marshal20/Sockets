#include "Network/Address.hpp"
#include "errors.hpp"
#include "sockImpl.hpp"
#include "util.hpp"
#include <string.h>

namespace nt
{

	void addrinfoTosockaddrstorage(const addrinfo* src, sockaddr_storage* dst)
	{
		memset(dst, 0, sizeof(sockaddr_storage));
		int struct_size = (src->ai_family == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
		memcpy(dst, src->ai_addr, struct_size);
	}

	Address::Address()
	{

	}

	Address::Address(const IPv4& value)
	{
		m_valid = true;
		m_addr.family = Family::IPv4;
		m_addr.v4 = value;
	}

	Address::Address(const IPv6& value)
	{
		m_valid = true;
		m_addr.family = Family::IPv6;
		m_addr.v6 = value;
	}

	Address::Address(const std::string& presentation) :
		Address(FromPresentationAll(presentation)[0])
	{
	}

	Address::~Address()
	{
	}

	std::ostream& operator<<(std::ostream& os, const Address& addr)
	{
		return os << addr.GetPresentation();
	}

	Family Address::GetFamily() const
	{
		return m_addr.family;
	}

	std::string Address::GetPresentation() const
	{
		if (!m_valid) return "Invalid address";

		sockaddr_storage temp_addr;
		create_sockaddr_from_address(*this, 0, &temp_addr);

		static const int temp_len = INET6_ADDRSTRLEN;
		static char temp[temp_len + 1];
		void* addr;

		if (temp_addr.ss_family == AF_INET)
		{
			struct sockaddr_in* ipv4 = (struct sockaddr_in*)&temp_addr;
			addr = &(ipv4->sin_addr);
		}
		else
		{
			struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)&temp_addr;
			addr = &(ipv6->sin6_addr);
		}

		inet_ntop(temp_addr.ss_family, addr, temp, temp_len);

		temp[temp_len] = '\0';

		return std::string(temp);
	}

	std::vector<Address> Address::FromPresentationAll(const std::string& rep)
	{
		std::vector<Address> addrs;

		struct addrinfo hints, *res, *it;
		const char* domainname = rep.c_str();

		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		int code = socketimpl::getaddrinfo(domainname, NULL, &hints, &res);
		if (code != 0)
			Error::runtime("getaddrinfo failed", gai_strerror(code), code);

		for (it = res; it != NULL; it = it->ai_next)
		{
			Address temp;
			unsigned short dummy_port = 0;
			sockaddr_storage temp_sockaddr_storage;
			addrinfoTosockaddrstorage(it, &temp_sockaddr_storage);
			create_address_from_sockaddr(temp, dummy_port, &temp_sockaddr_storage);
			temp.m_valid = true;
			addrs.push_back(temp);
		}

		freeaddrinfo(res);

		return addrs;
	}

	Address Address::Localhost(Family family)
	{
		if (family == Family::IPv4)
			return create_address_from_ipv4addr(htonl(INADDR_LOOPBACK));

		return create_address_from_ipv6addr(&in6addr_loopback);
	}

	Address Address::Thishost(Family family)
	{
		if (family == Family::IPv4)
			return create_address_from_ipv4addr(htonl(INADDR_ANY));

		return create_address_from_ipv6addr(&in6addr_any);
	}

	Address Address::Broadcast()
	{
		return create_address_from_ipv4addr(htonl(INADDR_BROADCAST));
	}

}
