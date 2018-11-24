#pragma once
#include <Network/Address.hpp>
#include <Network/EndPoint.hpp>
#include <Network/types.hpp>
#include <utility>

namespace nt
{

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

		void connect(const EndPoint& endpoint);
		void bind(const EndPoint& endpoint);
		void listen(int prelog = 10);
		std::pair<Socket, EndPoint> accept();
		int recv(void* buff, int len);
		int send(const void* buff, int len);
		std::pair<int, EndPoint> recvfrom(void* buff, int len);
		int sendto(const void* buff, int len, const EndPoint& endpoint);

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


}

