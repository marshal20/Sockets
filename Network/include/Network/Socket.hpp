#pragma once
#include <Network/Address.hpp>
#include <Network/types.hpp>
#include <utility>
#include <tuple>

class Socket
{
public:
	enum class Type
	{
		Stream = 1,
		Dgram
	};

	Socket(Type type = Type::Stream, Protocol family = Protocol::IPv4);
	~Socket();

	void close();

	void connect(const Address& addr);
	void bind(const Address& addr);
	void listen(int prelog = 10);
	std::tuple<Socket, Address> accept();
	int recv(void* buff, int len);
	int send(const void* buff, int len);
	int recvfrom(void* buff, int len, Address& sender);
	int sendto(const void* buff, int len, const Address& target);

	int getTotalrecv() const;
	int getTotalsent() const;

private:
	int m_sock = -1;
	Type m_type = Type::Stream;
	Protocol m_protocol = Protocol::IPv4;
	struct Monitor
	{
		int recv;
		int sent;
	} m_monitor = { 0, 0 };
};

