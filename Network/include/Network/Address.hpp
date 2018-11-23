#pragma once
#include <Network/types.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <utility>

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
	Address(const IPv4& value);
	Address(const IPv6& value);
	~Address();

	friend std::ostream& operator<<(std::ostream& os, const Address& addr);

	Family getFamily() const;
	std::string getPresentation() const;

	static std::vector<Address> fromPresentationAll(const std::string& rep);
	static Address fromPresentation(const std::string& rep);
	static Address localhost(Family family = Family::IPv4);
	static Address thishost(Family family = Family::IPv4);
	static Address broadcast();

private:
	friend class Socket;
	friend void create_sockaddr_from_address(const Address&, unsigned short, struct sockaddr_storage*);

private:
	Addr m_addr;
	bool m_valid = false;
};
