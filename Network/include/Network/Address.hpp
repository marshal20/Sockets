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
	Protocol type;
	union
	{
		IPv4 v4;
		IPv6 v6;
	};
	unsigned short port;
};

class Address
{
public:
	Address();
	~Address();

	static const Address localhost;
	static const Address broadcast;

	friend std::ostream& operator<<(std::ostream& os, const Address& addr);

	unsigned short getPort() const;
	Address setPort(unsigned short value);

	Address setIP(const IPv4& value);
	Address setIP(const IPv6& value);

	Protocol getProtocol() const;

	std::string getPresentation() const;
	static std::vector<Address> fromPresentationAll(const std::string& rep);
	static Address fromPresentation(const std::string& rep);

private:
	friend void AddressTosockaddr(const Address& val, sockaddr_storage* sockaddr);
	friend void sockaddrToAddress(Address& val, const sockaddr_storage* sockaddr);

private:
	friend class Socket;
	Addr m_addr;
	bool m_valid = false;
};
