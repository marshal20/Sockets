#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

int get_port(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return ntohs(((struct sockaddr_in*)sa)->sin_port);

    return ntohs(((struct sockaddr_in6*)sa)->sin6_port);
}

int main(int argc, char* argv[])
{
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
	while((sock_new = accept(sock_local, &their_addr, sizeof(their_addr))) != -1)
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
			if((cursent = send(sock_new, buff, sizeof(buff), 0)) == -1)
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
