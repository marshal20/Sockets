#include <Network/Network.hpp>
#include <iostream>
#include <string>
#include <tuple>
#include <thread>

using namespace nt; // Not recommended.

void session(Socket remote_socket, EndPoint remote_endpoint) try
{
	// new connection
	std::cout << "- Info: new session, Address: " 
		<< remote_endpoint.address << ":" << remote_endpoint.port << std::endl;

	char buff[1024];
	int currec;
	while ((currec = remote_socket.recv(buff, sizeof(buff))) != 0)
	{
		remote_socket.send(buff, currec);
	}

	printf("- Info: connection closed, receved: %d bytes, sent: %d bytes\n",
		remote_socket.getTotalrecv(), remote_socket.getTotalsent());
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
}

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

	while (true)
	{
		// Accept the new connection
		Socket new_sock; EndPoint new_endpoint;
		std::tie(new_sock, new_endpoint) = sock.accept();
		// Create a session thread then detach it.
		std::thread(session, new_sock, new_endpoint).detach();
	}

	sock.close();
	return 0;
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
	return 1;
}
