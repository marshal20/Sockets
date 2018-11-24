#include <Network/Network.hpp>
#include <iostream>
#include <thread>
#include <utility>

using namespace nt; // Not recommended.

EndPoint server_address;

void session(Socket client_socket, EndPoint client_address) try
{
	char buffer[2048];
	int recved;
	Socket server_socket(Socket::Type::Stream, server_address.address.GetFamily());

	std::cout << "- New connection: " << client_address << std::endl;
	server_socket.Connect(server_address);

	while (true)
	{
		recved = client_socket.Recv(buffer, sizeof(buffer));
		if (recved == 0)
			break;

		server_socket.Send(buffer, recved);

		recved = server_socket.Recv(buffer, sizeof(buffer));
		if (recved == 0)
			break;

		client_socket.Send(buffer, recved);
	}
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
}

int main(int argc, char* argv[]) try
{
	EndPoint host_address;

	if (argc != 5)
	{
		std::cout << "- Usage: Proxy <hostip> <hostport> <serverip> <serverport>" << std::endl;
		return -1;
	}

	host_address = { Address(argv[1]), (unsigned short)std::stoi(argv[2]) };
	server_address = { Address(argv[3]), (unsigned short)std::stoi(argv[4]) };

	Socket host_listiner(Socket::Type::Stream, host_address.address.GetFamily());

	host_listiner.Bind(host_address);
	std::cout << "- Info: socket bound to " << host_address << std::endl;
	host_listiner.Listen();

	while (true)
	{
		Socket client_sock; EndPoint client_addr;
		std::tie(client_sock, client_addr) = host_listiner.Accept();

		std::thread(session, client_sock, client_addr).detach();
	}

	return 0;
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
	return 1;
}
