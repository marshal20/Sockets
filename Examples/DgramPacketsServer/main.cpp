#include <Network/Network.hpp>
#include <iostream>
#include <tuple>
#include <stdio.h>
#include <string.h>

using namespace nt; // Not recommended.

int main(int argc, char* argv[]) try
{
	const char* port = "2000";
	if (argc > 1)
		port = argv[1];

	Family addrProtocol = Family::IPv4;
	if (argc > 2)
		if (argv[2][0] == '6')
			addrProtocol = Family::IPv6;

	Address localhostTarget = Address::Thishost(addrProtocol);
	Socket sock(Socket::Type::Dgram, localhostTarget.GetFamily());

	sock.Bind({ localhostTarget, (unsigned short)std::stoi(port) });
	std::cout << "- Info: socket bound to " << localhostTarget << " port: " << std::stoi(port) << std::endl;

	EndPoint remote;
	char buff[1025];
	int recvd;
	while (true)
	{
		std::tie(recvd, remote) = sock.RecvFrom(buff, sizeof(buff) - 1);
		if (recvd == 0) break;

		buff[recvd] = 0;
		std::cout << "- Recieved " << recvd << " Bytes, from: " << remote << ", buff: " <<  buff << std::endl;
		sock.SendTo(buff, recvd, remote);
	}

	sock.Close();
	return 0;
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
	return 1;
}
