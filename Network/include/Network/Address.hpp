#pragma once
#include <string>
#include <vector>

#if WIN32
#include <ws2tcpip.h>
#include <ws2def.h>
#else if __linux__
#include <sys/types.h>
#endif

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
	void setPort(short value);

	void setIP(IPv4 value);
	void setIP(IPv6 value);

	std::string getPresentation();
	static std::vector<Address> fromPresentationAll(const std::string& rep);
	static Address fromPresentation(const std::string& rep);
	static Address localhost();
	static Address broadcast();

private:
	sockaddr_storage m_addr;
};