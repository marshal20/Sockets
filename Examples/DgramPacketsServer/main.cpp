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
	sock.bind(localhostTarget);

	Address remoteaddr;
	char buff[1025];
	int recvd;
	while ((recvd = sock.recvfrom(buff, sizeof(buff) - 1, remoteaddr)) != 0)
	{
		buff[recvd] = 0;
		std::cout << "- Recieved " << recvd << " Bytes, from: " << remoteaddr << ", buff: " <<  buff << std::endl;
		sock.sendto(buff, recvd, remoteaddr);
	}


	return 0;
}