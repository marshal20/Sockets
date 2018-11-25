#include "Network/Socket.hpp"
#include "errors.hpp"
#include "sockImpl.hpp"
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include "util.hpp"

namespace nt
{

	int get_protocol_length(Family family) {
		return (family == Family::IPv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
	}

	Socket::Socket(Type type, Family family) :
		m_type(type), m_family(family)
	{
		int af = (family == Family::IPv4) ? PF_INET : PF_INET6;
		int t = (type == Type::Stream) ? SOCK_STREAM : SOCK_DGRAM;

		if ((m_sock = socketimpl::socket(af, t, 0)) == -1)
			Error::runtime("socket failed", errno);
	}

	Socket::~Socket()
	{
	}

	Socket::operator bool() const
	{
		return (m_sock == -1) ? false : true;
	}

	void Socket::Shutdown(How how)
	{
		if (m_sock == -1) return;

		int how_pram = (how == How::Read) ? SHUT_RD : ((how == How::Write) ? SHUT_WR : SHUT_RDWR);
		if (::shutdown(m_sock, how_pram) == -1)
			Error::runtime("shutdown failed", errno);
	}

	void Socket::Close()
	{
		if (m_sock == -1) return;
		if (::close(m_sock) == -1)
			Error::runtime("close failed", errno);
	}

	void Socket::BeBroadcast()
	{
		if (m_type != Type::Dgram) Error::runtime("can't make a stream socket broadcast");

		int broadcast = 1;
		if (setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast)) == -1)
			Error::runtime("setsockopt failed to set socket to broadcast", errno);
	}

	void Socket::Connect(const EndPoint& endpoint)
	{
		int len = get_protocol_length(endpoint.address.m_addr.family);
		sockaddr_storage temp_sockaddr_storage;
		create_sockaddr_from_address(endpoint.address, endpoint.port, &temp_sockaddr_storage);
		if (::connect(m_sock, (const sockaddr*)&temp_sockaddr_storage, len) == -1)
			Error::runtime("connect failed", errno);
	}

	void Socket::Bind(const EndPoint& endpoint)
	{
		int len = get_protocol_length(endpoint.address.m_addr.family);
		sockaddr_storage temp_sockaddr_storage;
		create_sockaddr_from_address(endpoint.address, endpoint.port, &temp_sockaddr_storage);
		if (::bind(m_sock, (const sockaddr*)&temp_sockaddr_storage, len) == -1)
			Error::runtime("Bind failed", errno);
	}

	void Socket::Listen(int prelog)
	{
		if (::listen(m_sock, prelog) == -1)
			Error::runtime("listen failed", errno);
	}

	std::pair<Socket, EndPoint> Socket::Accept()
	{
		Socket sock;
		Address addr;
		unsigned short port;

		addr.m_valid = true;
		int len = get_protocol_length(m_family);
		sockaddr_storage temp_sockaddr_storage;
		if ((sock.m_sock = ::accept(m_sock, (sockaddr*)&temp_sockaddr_storage, (socklen_t*)&len)) == -1)
			Error::runtime("accept failed", errno);

		create_address_from_sockaddr(addr, port, &temp_sockaddr_storage);

		return std::make_pair(sock, EndPoint({ addr, port }));
	}

	int Socket::Recv(char* buff, int len)
	{
		int recieved;
		if ((recieved = ::recv(m_sock, (char*)buff, len, 0)) == -1)
			Error::runtime("recv failed", errno);

		m_monitor.recv += recieved;
		return recieved;
	}

	int Socket::Send(const char* buff, int len)
	{
		int sent;
		if ((sent = ::send(m_sock, (const char*)buff, len, 0)) == -1)
			Error::runtime("send failed", errno);

		m_monitor.sent += sent;
		return sent;
	}

	void Socket::SendAll(const char* buff, int len)
	{
		int sent = 0;
		while (sent < len)
		{
			sent += Send((buff + sent), len - sent);
		}
	}

	std::pair<int, EndPoint> Socket::RecvFrom(char* buff, int len)
	{
		Address sender;
		unsigned short port;

		if (m_type != Type::Dgram) Error::runtime("call to recvfrom with non Dgram socket");

		int recvd;
		int len_addr = get_protocol_length(m_family);
		sockaddr_storage temp_sockaddr_storage;
		if ((recvd = ::recvfrom(m_sock, (char*)buff, len, 0, (sockaddr*)&temp_sockaddr_storage, (socklen_t*)&len_addr)) == -1)
			Error::runtime("recvfrom failed", errno);

		create_address_from_sockaddr(sender, port, &temp_sockaddr_storage);
		sender.m_valid = true;

		return std::make_pair(recvd, EndPoint({ sender, port }));
	}

	int Socket::SendTo(const char* buff, int len, const EndPoint& endpoint)
	{
		if (m_type != Type::Dgram) Error::runtime("call to sendto with non Dgram socket");

		int len_addr = get_protocol_length(endpoint.address.m_addr.family);
		sockaddr_storage temp_sockaddr_storage;
		create_sockaddr_from_address(endpoint.address, endpoint.port, &temp_sockaddr_storage);

		int sent;
		if ((sent = ::sendto(m_sock, (const char*)buff, len, 0, (const sockaddr*)&temp_sockaddr_storage, len_addr)) == -1)
			Error::runtime("sendto failed", errno);

		m_monitor.sent += sent;
		return sent;
	}

	EndPoint Socket::GetPeer() const
	{
		Address peer_address;
		unsigned short peer_port;
		sockaddr peer_sockaddr;
		socklen_t peer_sockaddr_len;

		if (::getpeername(m_sock, &peer_sockaddr, &peer_sockaddr_len) == -1)
			Error::runtime("getpeer failed", errno);

		create_address_from_sockaddr(peer_address, peer_port, (const sockaddr_storage*)&peer_sockaddr);

		return { peer_address, peer_port };
	}

	int Socket::GetTotalRecv() const {
		return m_monitor.recv;
	}

	int Socket::GetTotalSent() const {
		return m_monitor.sent;
	}

}
