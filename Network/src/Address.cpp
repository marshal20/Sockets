#include "Network/Address.hpp"
#include "errors.hpp"
#include "sockImpl.hpp"
#include <string.h>

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

std::ostream& operator<<(std::ostream& os, const Address& addr)
{
	return os << addr.getPresentation();
}

Address Address::setIP(const IPv4& value)
{
	m_valid = true;
	m_addr.family = Family::IPv4;
	m_addr.v4 = value;
	return *this;
}

Address Address::setIP(const IPv6& value)
{
	m_valid = true;
	m_addr.family = Family::IPv6;
	m_addr.v6 = value;
	return *this;
}

Family Address::getFamily() const
{
	return m_addr.family;
}

std::string Address::getPresentation() const
{
	if (!m_valid) return "Invalid address";

	sockaddr_storage temp_addr;
	AddressTosockaddr(*this, 0, &temp_addr);

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
		unsigned short dummy_port = 0;
		sockaddr_storage temp_sockaddr_storage;
		addrinfoTosockaddrstorage(it, &temp_sockaddr_storage);
		sockaddrToAddress(temp, dummy_port, &temp_sockaddr_storage);
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

Address Address::localhost(Family family)
{
	if(family == Family::IPv4)
		return fromNetworkInt(INADDR_LOOPBACK);

	return Addressfromin6addr(&in6addr_loopback);
}

Address Address::thishost(Family family)
{
	if (family == Family::IPv4)
		return fromNetworkInt(INADDR_ANY);

	return Addressfromin6addr(&in6addr_any);
}

Address Address::broadcast()
{
	return fromNetworkInt(INADDR_BROADCAST);
}

Address Address::fromNetworkInt(int val)
{
	Address temp;
	temp.m_valid = true;
	*(int*)&temp.m_addr.v4 = ntohl(val);
	temp.m_addr.family = Family::IPv4;
	return temp;
}

Address Address::Addressfromin6addr(const struct in6_addr* in6addr)
{
	Address temp;
	unsigned short dummy_port = 0;
	temp.m_valid = true;
	sockaddr_in6 temp_addr6;
	memset(&temp_addr6, 0, sizeof(temp_addr6));
	temp_addr6.sin6_family = PF_INET6;
	memcpy(&temp_addr6.sin6_addr, in6addr, sizeof(in6_addr));
	sockaddrToAddress(temp, dummy_port,(const sockaddr_storage*)&temp_addr6);
	return temp;
}

void AddressTosockaddr(const Address& val, unsigned short port, sockaddr_storage* sockaddr)
{
	if (val.m_addr.family == Family::IPv4)
	{
		sockaddr->ss_family = PF_INET;
		sockaddr_in* temp = (sockaddr_in*)sockaddr;
		unsigned char v4_interm[4] = { val.m_addr.v4.a, val.m_addr.v4.b, val.m_addr.v4.c, val.m_addr.v4.d };
		memset(&temp->sin_addr, 0, sizeof(sockaddr_in));
		memcpy(&temp->sin_addr, v4_interm, sizeof(v4_interm));
		temp->sin_port = htons(port);
		return;
	}

	// IPv6
	sockaddr->ss_family = PF_INET6;
	sockaddr_in6* temp = (sockaddr_in6*)sockaddr;
	unsigned short v6_interm[8] = { htons(val.m_addr.v6.a), htons(val.m_addr.v6.b), htons(val.m_addr.v6.c), htons(val.m_addr.v6.d),
		htons(val.m_addr.v6.e), htons(val.m_addr.v6.f), htons(val.m_addr.v6.g), htons(val.m_addr.v6.h) };
	memset(&temp->sin6_addr, 0, sizeof(sockaddr_in6));
	memcpy(&temp->sin6_addr, v6_interm, sizeof(v6_interm));
	temp->sin6_port = htons(port);
}

void sockaddrToAddress(Address& val, unsigned short& port, const sockaddr_storage* sockaddr)
{
	if (sockaddr->ss_family == PF_INET)
	{
		val.m_addr.family = Family::IPv4;
		sockaddr_in* temp = (sockaddr_in*)sockaddr;
		unsigned char* v4_interm = (unsigned char*)&temp->sin_addr;
		val.m_addr.v4.a = v4_interm[0]; val.m_addr.v4.b = v4_interm[1];
		val.m_addr.v4.c = v4_interm[2]; val.m_addr.v4.d = v4_interm[3];
		port = ntohs(temp->sin_port);
		return;
	}

	// IPv6
	val.m_addr.family = Family::IPv6;
	sockaddr_in6* temp = (sockaddr_in6*)sockaddr;
	unsigned short* v6_interm = (unsigned short*)&temp->sin6_addr;
	// make an array of unsigned short so that we don't worry about endians
	val.m_addr.v6.a = ntohs(v6_interm[0]); val.m_addr.v6.b = ntohs(v6_interm[1]);
	val.m_addr.v6.c = ntohs(v6_interm[2]); val.m_addr.v6.d = ntohs(v6_interm[3]);
	val.m_addr.v6.e = ntohs(v6_interm[4]); val.m_addr.v6.f = ntohs(v6_interm[5]);
	val.m_addr.v6.g = ntohs(v6_interm[6]); val.m_addr.v6.h = ntohs(v6_interm[7]);

	port = ntohs(temp->sin6_port);
}

