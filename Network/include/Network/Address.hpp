#pragma once
#include <string>
#include <vector>

#if WIN32
#include <ws2tcpip.h>
#include <ws2def.h>
#else if __linux__
#include <sys/types.h>
#include <sys/socket.h>
#endif
#include <Network/types.hpp>

struct IPv4 {
	union {
		struct {
			unsigned char a, b, c, d;
		};
		unsigned int val;
	};
};

struct IPv6 {
	unsigned short a, b, c, d, e, f, g, h;
};

class Address
{
public:
	Address();
	~Address();

	unsigned short getPort() const;
	Address setPort(short value);

	void setIP(const IPv4& value);
	void setIP(const IPv6& value);

	Protocol getProtocol() const;

	std::string getPresentation() const;
	static std::vector<Address> fromPresentationAll(const std::string& rep);
	static Address fromPresentation(const std::string& rep);
	static Address localhost();
	static Address broadcast();

private:
	friend class Socket;
	sockaddr_storage m_addr;
	bool m_valid = false;
};
