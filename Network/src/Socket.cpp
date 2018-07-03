#include "Network/Socket.hpp"
#include "errors.hpp"
#include "sockImpl.hpp"
#include <sys/types.h>
#include <errno.h>

Socket::Socket(Type type, Protocol family)
{
	int af = (family == Protocol::IPv4) ? PF_INET : PF_INET6;
	int t = (type == Type::Stream) ? SOCK_STREAM : SOCK_DGRAM;
	m_sock = sockImpl::socket(af, t, 0);
	if (m_sock == -1)
		Error::runtime("Invalid socket", m_sock);
}

Socket::~Socket()
{
	/*
	if (closesocket(m_sock) == -1)
		Error::runtime("closesocket failed", m_sock);
	*/
}

void Socket::bind(const Address& addr)
{
	int len = (addr.getProtocol() == Protocol::IPv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
	if (::bind(m_sock, (const sockaddr*)&(addr.m_addr), len) == -1)
		Error::runtime("Bind failed", errno);
}

void Socket::listen(int prelog)
{
	if (::listen(m_sock, prelog) == -1)
		Error::runtime("listen failed", errno);
}

std::tuple<Socket, Address> Socket::accept()
{
	Socket sock;
	Address addr;
	addr.m_valid = true;
	int len = (m_protocol == Protocol::IPv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
	if ((sock.m_sock = ::accept(m_sock, (sockaddr*)&addr.m_addr, (socklen_t*)&len)) == -1)
		Error::runtime("accept failed", errno);

	return std::make_tuple(sock, addr);
}

int Socket::recv(void* buff, int len)
{
	int recieved;
	if((recieved = ::recv(m_sock, (char*)buff, len, 0)) == -1)
		Error::runtime("recv failed", errno);

	return recieved;
}

int Socket::send(const void* buff, int len)
{
	int sent;
	if ((sent = ::send(m_sock, (const char*)buff, len, 0)) == -1)
		Error::runtime("send failed", errno);

	return sent;
}
