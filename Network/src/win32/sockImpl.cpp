#include "win32/sockImpl.hpp"
#include <exception>

#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int close(int sock)
{
	return closesocket(sock);
}

namespace socketimpl
{

	int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
	{
		return ::getaddrinfo(node, service, hints, res);
	}

	int socket(int af, int type, int protocol)
	{
		return ::socket(af, type, protocol);
	}

}


// https://docs.microsoft.com/en-us/windows/desktop/WinSock/complete-client-code
struct WindowsSocket2Init
{
	WindowsSocket2Init()
	{
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR) {
			throw std::exception("WSAStartup failed.\n", iResult);
		}
	}

	~WindowsSocket2Init()
	{
		WSACleanup();
	}
};

WindowsSocket2Init global_ws2init;