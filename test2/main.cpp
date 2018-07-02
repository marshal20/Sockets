#include <iostream>
#include <Network/Address.hpp>
#include <Network/Socket.hpp>

int main()
{
	Socket sock;
	sock.bind(Address::localhost().setPort(3000));
	std::cout << "- Info: socket bound to localhost port: " << 3000 << std::endl;
	sock.listen();
	Socket new_sock;
	Address new_addr;
	while (true)
	{
		std::tie(new_sock, new_addr) = sock.accept();

		// new connection
		std::cout << "- Info: new connection, Address: " << new_addr.getPresentation() << std::endl;
		
		int recvd = 0;
		int sent = 0;

		char buff[1024];
		int currec;
		while ((currec = new_sock.recv(buff, sizeof(buff))) != 0)
		{
			recvd += currec;
			int cursent;
			cursent = new_sock.send(buff, currec);
			sent += cursent;
		}

		printf("- Info: connection closed, receved: %d bytes, sent: %d bytes\n", recvd, sent);
	}

	return 0;
}
