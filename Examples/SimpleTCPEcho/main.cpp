#include <Network/Address.hpp>
#include <Network/Socket.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	const char* port = "3000";
	if (argc == 2)
		port = argv[1];

	Socket sock;
	Address localhostTarget = Address::thishost().setPort(std::stoi(port)); // thishost:port
	sock.bind(localhostTarget);
	std::cout << "- Info: socket bound to " << localhostTarget << " port: " << port << std::endl;
	sock.listen();

	Socket new_sock; Address new_addr;
	while (new_sock = sock.accept(new_addr))
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

	return 0;
}
