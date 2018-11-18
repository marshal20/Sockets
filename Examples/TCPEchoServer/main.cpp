#include <Network/Address.hpp>
#include <Network/Socket.hpp>
#include <iostream>
#include <string>

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

	sock.bind(localhostTarget, std::stoi(port));
	std::cout << "- Info: socket bound to " << localhostTarget << " port: " << port << std::endl;
	sock.listen();

	Socket new_sock; Address new_addr; unsigned short new_port;
	while (new_sock = sock.accept(new_addr, new_port))
	{
		// new connection
		std::cout << "- Info: new connection, Address: " << new_addr << std::endl;

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
