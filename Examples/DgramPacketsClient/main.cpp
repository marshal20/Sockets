#include <Network/Address.hpp>
#include <Network/Socket.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) try
{
	const char* domainname = "127.0.0.1";
	if (argc > 1)
		domainname = argv[1];

	const char* port = "2000";
	if (argc == 3)
		port = argv[2];

	Address remoteTarget = Address::fromPresentation(domainname).setPort(std::stoi(port));

	Socket sock(Socket::Type::Dgram, remoteTarget.getProtocol());
	sock.beBroadcast();

	char buff[1025];
	while (std::cin.getline(buff, sizeof(buff)))
	{
		sock.sendto(buff, strlen(buff) + 1, remoteTarget);
		std::cout << "- sent " << strlen(buff) + 1 << " Bytes, to: " << remoteTarget << ", buff: " << buff << std::endl;
	}

	sock.close();
	return 0;
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
}
