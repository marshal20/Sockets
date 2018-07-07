#include <Network/Address.hpp>
#include <Network/Socket.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	const char* port = "2000";
	if (argc == 2)
		port = argv[2];

	Address localhostTarget = Address::thishost().setPort(std::stoi(port));
	Socket sock(Socket::Type::Dgram, localhostTarget.getProtocol());

	sock.bind(localhostTarget);
	std::cout << "- Info: socket bound to " << localhostTarget << " port: " << localhostTarget.getPort() << std::endl;

	Address remoteaddr;
	char buff[1025];
	int recvd;
	while ((recvd = sock.recvfrom(buff, sizeof(buff) - 1, remoteaddr)) != 0)
	{
		buff[recvd] = 0;
		std::cout << "- Recieved " << recvd << " Bytes, from: " << remoteaddr << ", buff: " <<  buff << std::endl;
		sock.sendto(buff, recvd, remoteaddr);
	}

	sock.close();
	return 0;
}