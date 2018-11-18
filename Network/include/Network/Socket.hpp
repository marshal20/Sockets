#pragma once
#include <Network/Address.hpp>
#include <Network/types.hpp>
#include <utility>

class Socket
{
public:
	enum class Type
	{
		Stream = 1,
		Dgram
	};

	Socket(Type type = Type::Stream, Family family = Family::IPv4);
	~Socket();

	operator bool() const;

	void close();
	void beBroadcast();

	void connect(const Address& addr, unsigned short port);
	void bind(const Address& addr, unsigned short port);
	void listen(int prelog = 10);
	Socket accept(Address& remoteAddr, unsigned short& remotePort);
	int recv(void* buff, int len);
	int send(const void* buff, int len);
	int recvfrom(void* buff, int len, Address& sender, unsigned short& port);
	int sendto(const void* buff, int len, const Address& target, unsigned short port);

	int getTotalrecv() const;
	int getTotalsent() const;

private:
	int m_sock = -1;
	Type m_type = Type::Stream;
	Family m_family = Family::IPv4;
	struct Monitor
	{
		int recv;
		int sent;
	} m_monitor = { 0, 0 };
};

