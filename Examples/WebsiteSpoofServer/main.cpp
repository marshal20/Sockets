#include <Network/Address.hpp>
#include <Network/Socket.hpp>
#include <iostream>
#include <string>
#include <sstream>

void serveClient(Socket client_sock)
{

}

int main(int argc, char* argv[]) try
{
	const char* port = "8080";
	if (argc > 1)
		port = argv[1];

	const char* website = "example.com";
	if (argc > 2)
		website = argv[2];

	Address localhostTarget = Address::thishost(Protocol::IPv4).setPort(std::stoi(port)); // thishost:port
	Socket sock(Socket::Type::Stream, localhostTarget.getProtocol());

	sock.bind(localhostTarget);
	std::cout << "- Info: socket bound to " << localhostTarget << " port: " << port << std::endl;
	std::cout << "- Server will spoof: " << website << std::endl;
	sock.listen();

	Socket new_sock; Address new_addr;
	while (new_sock = sock.accept(new_addr))
	{
		// new connection
		std::cout << "- Info: new connection, Address: " << new_addr << std::endl;
		serveClient(new_sock);
	}

	sock.close();
	return 0;
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
	return 1;
}
