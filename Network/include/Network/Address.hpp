#pragma once
#include <Network/types.hpp>
#include <string>
#include <vector>
#include <iostream>

struct sockaddr_storage;


struct IPv4 {
	unsigned char a, b, c, d;
};

struct IPv6 {
	unsigned short a, b, c, d, e, f, g, h;
};

struct Addr
{
	Family family;
	union
	{
		IPv4 v4;
		IPv6 v6;
	};
};

class Address
{
public:
	Address();
	~Address();

	friend std::ostream& operator<<(std::ostream& os, const Address& addr);

	Address setIP(const IPv4& value);
	Address setIP(const IPv6& value);

	Family getFamily() const;

	std::string getPresentation() const;
	static std::vector<Address> fromPresentationAll(const std::string& rep);
	static Address fromPresentation(const std::string& rep);
	static Address localhost(Family family = Family::IPv4);
	static Address thishost(Family family = Family::IPv4);
	static Address broadcast();

private:
	static Address fromNetworkInt(int val);
	static Address Addressfromin6addr(const struct in6_addr* in6addr);

private:
	friend void AddressTosockaddr(const Address& val, unsigned short port, sockaddr_storage* sockaddr);
	friend void sockaddrToAddress(Address& val, unsigned short& port, const sockaddr_storage* sockaddr);

private:
	friend class Socket;
	Addr m_addr;
	bool m_valid = false;
};
