#include <Network/Address.hpp>
#include <Network/Socket.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	std::cout << Address().setIP(IPv4({ 127,0,0,1 })).getPresentation() << std::endl;
	std::cout << Address().setIP(IPv6({ 1,2,3,4,5,6,7,8 })).getPresentation() << std::endl;
	std::cin.get();

	char* port = "3000";
	if (argc == 2)
		port = argv[1];

	Socket sock;
	Address localhostTarget = Address::localhost().setPort(std::stoi(port));
	sock.bind(localhostTarget);
	std::cout << "- Info: socket bound to " << localhostTarget.getPresentation() << " port: " << port << std::endl;
	sock.listen();
	Socket new_sock;
	Address new_addr;
	while (true)
	{
		std::tie(new_sock, new_addr) = sock.accept();

		// new connection
		std::cout << "- Info: new connection, Address: " << new_addr.getPresentation() << std::endl;

		char buff[1024];
		int currec;
		while ((currec = new_sock.recv(buff, sizeof(buff))) != 0)
		{
			new_sock.send(buff, currec);
		}

		printf("- Info: connection closed, receved: %d bytes, sent: %d bytes\n",
			new_sock.getTotalrecv(), new_sock.getTotalsent());
	}

	return 0;
}
