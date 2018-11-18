#include <Network/Address.hpp>
#include <Network/Socket.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) try
{
	const char* port = "2000";
	if (argc > 1)
		port = argv[1];

	Family addrProtocol = Family::IPv4;
	if (argc > 2)
		if (argv[2][0] == '6')
			addrProtocol = Family::IPv6;

	Address localhostTarget = Address::thishost(addrProtocol);
	Socket sock(Socket::Type::Dgram, localhostTarget.getFamily());

	sock.bind(localhostTarget, std::stoi(port));
	std::cout << "- Info: socket bound to " << localhostTarget << " port: " << std::stoi(port) << std::endl;

	Address remoteaddr;
	unsigned short remoteport;
	char buff[1025];
	int recvd;
	while ((recvd = sock.recvfrom(buff, sizeof(buff) - 1, remoteaddr, remoteport)) != 0)
	{
		buff[recvd] = 0;
		std::cout << "- Recieved " << recvd << " Bytes, from: " << remoteaddr << ", buff: " <<  buff << std::endl;
		sock.sendto(buff, recvd, remoteaddr, remoteport);
	}

	sock.close();
	return 0;
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
	return 1;
}
