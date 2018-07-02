#include "Network/Socket.hpp"
#include "sockImpl.hpp"
#include <exception>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <errno.h>

#if WIN32
typedef int socklen_t;
#endif

Socket::Socket(Type type, Protocol family)
{
	int af = (family == Protocol::IPv4) ? PF_INET : PF_INET6;
	int t = (type == Type::Stream) ? SOCK_STREAM : SOCK_DGRAM;
	m_sock = sockImpl::socket(af, t, 0);
	if (m_sock == -1)
	{
		std::string msg = std::string("Invalid socket") + std::to_string(m_sock);
		throw std::runtime_error(msg.c_str());
	}
}

Socket::~Socket()
{
	/*
	if (closesocket(m_sock) == -1)
		throw std::runtime_error("Socket failed to close", m_sock);
		*/
}

void Socket::bind(const Address& addr)
{
	int len = (addr.getProtocol() == Protocol::IPv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
	if (::bind(m_sock, (const sockaddr*)&(addr.m_addr), len) == -1)
	{
		std::string msg = std::string("Bind error: ") + std::to_string(errno);
		throw std::runtime_error(msg.c_str());
	}
}

void Socket::listen(int prelog)
{
	if (::listen(m_sock, prelog) == -1)
	{
		std::string msg = std::string("listen error: ") + std::to_string(errno);
		throw std::runtime_error(msg.c_str());
	}
}

std::tuple<Socket, Address> Socket::accept()
{
	Socket sock;
	Address addr;
	addr.m_valid = true;
	int len = (m_protocol == Protocol::IPv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
	if ((sock.m_sock = ::accept(m_sock, (sockaddr*)&addr.m_addr, (socklen_t*)&len)) == -1)
	{
		std::string msg = std::string("accept error: ") + std::to_string(errno);
		throw std::runtime_error(msg.c_str());
	}

	// convert return to pair
	return std::make_tuple(sock, addr);
}

int Socket::recv(void* buff, int len)
{
	int recieved;
	if((recieved = ::recv(m_sock, (char*)buff, len, 0)) == -1)
	{
		std::string msg = std::string("recv error: ") + std::to_string(errno);
		throw std::runtime_error(msg.c_str());
	}
	return recieved;
}

int Socket::send(const void* buff, int len)
{
	int sent;
	if ((sent = ::send(m_sock, (const char*)buff, len, 0)) == -1)
	{
		std::string msg = std::string("send error: ") + std::to_string(errno);
		throw std::runtime_error(msg.c_str());
	}
	return sent;
}
