#include "Network/Address.hpp"
#include "errors.hpp"
#include "sockImpl.hpp"
#include <string.h>

const Address Address::localhost = Address().setIP(IPv4({ 127, 0, 0, 1 }));;
const Address Address::broadcast = Address().setIP(IPv4({ 255, 255, 255, 255 }));;

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

unsigned short Address::getPort() const
{
	return m_addr.port;
}

Address Address::setPort(unsigned short value)
{
	m_addr.port = value;
	return *this;
}

Address Address::setIP(const IPv4& value)
{
	m_valid = true;
	m_addr.type = Protocol::IPv4;
	m_addr.v4 = value;
	return *this;
}

Address Address::setIP(const IPv6& value)
{
	m_valid = true;
	m_addr.type = Protocol::IPv6;
	m_addr.v6 = value;
	return *this;
}

Protocol Address::getProtocol() const
{
	return m_addr.type;
}

std::string Address::getPresentation() const
{
	if (!m_valid) return "Invalid address";

	sockaddr_storage temp_addr;
	AddressTosockaddr(*this, &temp_addr);

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
		sockaddr_storage temp_sockaddr_storage;
		addrinfoTosockaddrstorage(it, &temp_sockaddr_storage);
		sockaddrToAddress(temp, &temp_sockaddr_storage);
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
	unsigned short v6_interm[8] = { htons(val.m_addr.v6.a), htons(val.m_addr.v6.b), htons(val.m_addr.v6.c), htons(val.m_addr.v6.d),
		htons(val.m_addr.v6.e), htons(val.m_addr.v6.f), htons(val.m_addr.v6.g), htons(val.m_addr.v6.h) };
	memcpy(&temp->sin6_addr, v6_interm, sizeof(v6_interm));
	temp->sin6_port = htons(val.m_addr.port);
}

void sockaddrToAddress(Address& val, const sockaddr_storage* sockaddr)
{
	if (sockaddr->ss_family == PF_INET)
	{
		val.m_addr.type = Protocol::IPv4;
		sockaddr_in* temp = (sockaddr_in*)sockaddr;
		unsigned char* v4_interm = (unsigned char*)&temp->sin_addr;
		val.m_addr.v4.a = v4_interm[0]; val.m_addr.v4.b = v4_interm[1];
		val.m_addr.v4.c = v4_interm[2]; val.m_addr.v4.d = v4_interm[3];
		val.m_addr.port = ntohs(temp->sin_port);
		return;
	}

	// IPv6
	val.m_addr.type = Protocol::IPv6;
	sockaddr_in6* temp = (sockaddr_in6*)sockaddr;
	unsigned short* v6_interm = (unsigned short*)&temp->sin6_addr;
	// make an array of unsigned short so that we don't worry about endians
	val.m_addr.v6.a = ntohs(v6_interm[0]); val.m_addr.v6.b = ntohs(v6_interm[1]);
	val.m_addr.v6.c = ntohs(v6_interm[2]); val.m_addr.v6.d = ntohs(v6_interm[3]);
	val.m_addr.v6.e = ntohs(v6_interm[4]); val.m_addr.v6.f = ntohs(v6_interm[5]);
	val.m_addr.v6.g = ntohs(v6_interm[6]); val.m_addr.v6.h = ntohs(v6_interm[7]);

	val.m_addr.port = ntohs(temp->sin6_port);
}

