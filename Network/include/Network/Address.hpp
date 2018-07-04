#pragma once
#include <string>
#include <vector>

/*
#if WIN32
#include <ws2tcpip.h>
#include <ws2def.h>
#else if __linux__
#include <sys/types.h>
#include <sys/socket.h>
#endif
*/
#include <Network/types.hpp>

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

	unsigned short getPort() const;
	Address setPort(short value);

	Address setIP(const IPv4& value);
	Address setIP(const IPv6& value);

	Protocol getProtocol() const;

	std::string getPresentation() const;
	static std::vector<Address> fromPresentationAll(const std::string& rep);
	static Address fromPresentation(const std::string& rep);
	static Address localhost();
	static Address broadcast();

private:
	friend void AddressTosockaddr(const Address& val, sockaddr_storage* sockaddr);
	friend void sockaddrToAddress(Address& val, const sockaddr_storage* sockaddr);

private:
	friend class Socket;
	//sockaddr_storage m_addr;
	Addr m_addr;
	bool m_valid = false;
};
