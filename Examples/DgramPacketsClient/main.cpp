#include <Network/Address.hpp>
#include <Network/Socket.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	const char* domainname = "127.0.0.1";
	if (argc > 1)
		domainname = argv[1];

	const char* port = "2000";
	if (argc == 3)
		port = argv[2];

	Address localhostTarget = Address::fromPresentation(domainname);
	localhostTarget.setPort(std::stoi(port));

	Socket sock(Socket::Type::Dgram);
	sock.beBroadcast();

	char buff[1025];
	while (std::cin.getline(buff, sizeof(buff)))
	{
		sock.sendto(buff, strlen(buff) + 1, localhostTarget);
		std::cout << "- sent " << strlen(buff) + 1 << " Bytes, to: " << localhostTarget << ", buff: " << buff << std::endl;
	}

	return 0;
}