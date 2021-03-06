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

		enum class How
		{
			Read = 1,
			Write,
			ReadWrite
		};

		Socket(Type type = Type::Stream, Family family = Family::IPv4);
		~Socket();

		operator bool() const;

		void Shutdown(How how);
		void Close();
		void BeBroadcast();

		void Connect(const EndPoint& endpoint);
		void Bind(const EndPoint& endpoint);
		void Listen(int prelog = 10);
		std::pair<Socket, EndPoint> Accept();

		int Recv(char* buff, int len);
		int Send(const char* buff, int len);
		void SendAll(const char* buff, int len);
		std::pair<int, EndPoint> RecvFrom(char* buff, int len);
		int SendTo(const char* buff, int len, const EndPoint& endpoint);

		EndPoint GetPeer() const;
		int GetTotalRecv() const;
		int GetTotalSent() const;

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

