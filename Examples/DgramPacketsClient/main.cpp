#include <Network/Address.hpp>
#include <Network/Socket.hpp>
#include <iostream>
#include <stdio.h>

int main(int argc, char* argv[])
{
	const char* port = "2000";
	if (argc == 2)
		port = argv[2];

	Address localhostTarget = Address::localhost;
	localhostTarget.setPort(std::stoi(port));

	Socket sock(Socket::Type::Dgram);

	char buff[1025];
	while (std::cin.getline(buff, sizeof(buff)))
	{
		sock.sendto(buff, strlen(buff) + 1, localhostTarget);
		std::cout << "- sent " << strlen(buff) + 1 << " Bytes, to: " << localhostTarget << ", buff: " << buff << std::endl;
	}

	return 0;
}