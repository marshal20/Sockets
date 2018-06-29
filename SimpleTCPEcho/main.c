#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef __linux__ 
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#else
#include <sys/types.h>
#include <winsock2.h>
#include <ws2def.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
typedef int socklen_t;
#endif



int get_port(struct sockaddr *sa)
{
#ifndef __linux__
	return ntohs(((struct sockaddr_in*)sa)->sin_port);
#else
    if (sa->sa_family == AF_INET)
        return ntohs(((struct sockaddr_in*)sa)->sin_port);

    return ntohs(((struct sockaddr_in6*)sa)->sin6_port);
#endif
}

int main(int argc, char* argv[])
{
#ifndef __linux__ 
	//----------------------
	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %ld\n", iResult);
		return 1;
	}
#endif


	char* port = "3000";
	if(argc == 2)
		port = argv[1];

	//printf("- TCP server bound on port %s\n", port);

	struct addrinfo hint, *res;
	int code;
	memset(&hint, 0, sizeof(struct addrinfo));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE;
	
	if((code = getaddrinfo(NULL, port, &hint, &res)) != 0)
	{
		printf("- Error: %s.\n", gai_strerror(code));
		return 1;
	}
	
	int sock_local;
	if((sock_local = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
	{
		printf("- Error: socket failed, errno=%d\n", errno);
		return 1;
	}
	
	// binding the socket
	if(bind(sock_local, res->ai_addr, res->ai_addrlen) == -1)
	{
		printf("- Error: bind failed, errno=%d\n", errno);
		return 1;
	}
	
	printf("- Info: socket bound to port: %d\n", get_port(res->ai_addr));
	
	if(listen(sock_local, 10) == -1)
	{
		printf("- Error: listen failed, errno=%d\n", errno);
		return 1;
	}
	
	int sock_new;
	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof(their_addr);
	while((sock_new = accept(sock_local, (struct sockaddr *)&their_addr, &addr_size)) != -1)
	{
		// new connection
		printf("- Info: new connection, socket: %d\n", sock_new);
		
		int recvd = 0;
		int sent = 0;
		
		char buff[1024];
		
		int currec;
		while((currec = recv(sock_new, buff, sizeof(buff), 0)) != 0)
		{
			recvd += currec;
			int cursent;
			if((cursent = send(sock_new, buff, currec, 0)) == -1)
			{
				printf("- Error: send failed, errno=%d\n", errno);
				return 1;
			}
			sent += cursent;
		}
		
		printf("- Info: connection closed, receved: %d bytes, sent: %d bytes\n", recvd, sent);
	}
	
	freeaddrinfo(res);
	return 0;
}
