#include <Network/Network.hpp>
#include <iostream>
#include <string>
#include <tuple>

int main(int argc, char* argv[]) try
{
	const char* port = "3000";
	if (argc == 2)
		port = argv[1];

	Family addrProtocol = Family::IPv4;
	if (argc > 2)
		if (argv[2][0] == '6')
			addrProtocol = Family::IPv6;

	Address localhostTarget = Address::thishost(addrProtocol); // thishost:port
	Socket sock(Socket::Type::Stream, localhostTarget.getFamily());

	sock.bind({ localhostTarget, (unsigned short)std::stoi(port) });
	std::cout << "- Info: socket bound to " << localhostTarget << " port: " << port << std::endl;
	sock.listen();

	Socket new_sock; EndPoint new_endpoint;
	while (true)
	{
		std::tie(new_sock, new_endpoint) = sock.accept();

		// new connection
		std::cout << "- Info: new connection, Address: " << new_endpoint.address << std::endl;

		char buff[1024];
		int currec;
		while ((currec = new_sock.recv(buff, sizeof(buff))) != 0)
		{
			new_sock.send(buff, currec);
		}

		printf("- Info: connection closed, receved: %d bytes, sent: %d bytes\n",
			new_sock.getTotalrecv(), new_sock.getTotalsent());
	}

	sock.close();
	return 0;
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
	return 1;
}
