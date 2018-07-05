#include "Network/Socket.hpp"
#include "errors.hpp"
#include "sockImpl.hpp"
#include <sys/types.h>
#include <errno.h>
#include <string.h>

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

void Socket::close()
{
	if (::close(m_sock) == -1)
		Error::runtime("close failed", errno);
}

void Socket::connect(const Address& addr)
{
	int len = (addr.m_addr.type == Protocol::IPv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
	sockaddr_storage temp_sockaddr_storage;
	AddressTosockaddr(addr, &temp_sockaddr_storage);
	if(::connect(m_sock, (const sockaddr*)&temp_sockaddr_storage, len) == -1)
		Error::runtime("connect failed", errno);
}

void Socket::bind(const Address& addr)
{
	int len = (addr.m_addr.type == Protocol::IPv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
	sockaddr_storage temp_sockaddr_storage;
	AddressTosockaddr(addr, &temp_sockaddr_storage);
	if (::bind(m_sock, (const sockaddr*)&temp_sockaddr_storage, len) == -1)
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
	sockaddr_storage temp_sockaddr_storage;
	if ((sock.m_sock = ::accept(m_sock, (sockaddr*)&temp_sockaddr_storage, (socklen_t*)&len)) == -1)
		Error::runtime("accept failed", errno);

	sockaddrToAddress(addr, &temp_sockaddr_storage);
	return std::make_tuple(sock, addr);
}

int Socket::recv(void* buff, int len)
{
	int recieved;
	if((recieved = ::recv(m_sock, (char*)buff, len, 0)) == -1)
		Error::runtime("recv failed", errno);

	m_monitor.recv += recieved;
	return recieved;
}

int Socket::send(const void* buff, int len)
{
	int sent;
	if ((sent = ::send(m_sock, (const char*)buff, len, 0)) == -1)
		Error::runtime("send failed", errno);

	m_monitor.sent += sent;
	return sent;
}

int Socket::recvfrom(void* buff, int len, Address& sender)
{
	if(m_type != Type::Dgram) Error::runtime("call to recvfrom with non Dgram socket");

	int recvd;
	int len_addr = (m_protocol == Protocol::IPv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
	sockaddr_storage temp_sockaddr_storage;
	if ((recvd = ::recvfrom(m_sock, (char*)buff, len, 0, (sockaddr*)&temp_sockaddr_storage, &len_addr)) == -1)
		Error::runtime("recvfrom failed", errno);

	sockaddrToAddress(sender, &temp_sockaddr_storage);
	return recvd;
}

int Socket::sendto(const void* buff, int len, const Address& target)
{
	if (m_type != Type::Dgram) Error::runtime("call to sendto with non Dgram socket");

	int len_addr = (target.m_addr.type == Protocol::IPv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
	sockaddr_storage temp_sockaddr_storage;
	AddressTosockaddr(target, &temp_sockaddr_storage);

	int sent;
	if ((sent = ::sendto(m_sock, (const char*)buff, len, 0, (const sockaddr*)&temp_sockaddr_storage, len_addr)) == -1)
		Error::runtime("sendto failed", errno);

	m_monitor.sent += sent;
	return sent;
}


int Socket::getTotalrecv() const {
	return m_monitor.recv;
}

int Socket::getTotalsent() const {
	return m_monitor.sent;
}
