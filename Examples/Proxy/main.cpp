#include <Network/Network.hpp>
#include <iostream>
#include <thread>
#include <utility>

using namespace nt; // Not recommended.

EndPoint server_address;

void upstream(Socket server_socket, Socket client_socket) try
{
	char buffer[8192];
	int recved;

	while (true)
	{
		recved = client_socket.Recv(buffer, sizeof(buffer));
		if (recved <= 0)
			break;
		server_socket.SendAll(buffer, recved);
	}

	client_socket.Shutdown(Socket::How::ReadWrite);
	server_socket.Shutdown(Socket::How::ReadWrite);
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
}

void downstream(Socket server_socket, Socket client_socket) try
{
	char buffer[8192];
	int recved;

	while (true)
	{
		recved = server_socket.Recv(buffer, sizeof(buffer));
		if (recved <= 0)
			break;
		client_socket.SendAll(buffer, recved);
	}

	client_socket.Shutdown(Socket::How::ReadWrite);
	server_socket.Shutdown(Socket::How::ReadWrite);
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
}

void session(Socket client_socket, EndPoint client_address) try
{
	Socket server_socket(Socket::Type::Stream, server_address.address.GetFamily());

	std::cout << "- New connection: " << client_address << std::endl;
	server_socket.Connect(server_address);

	std::thread upstream_thread(upstream, server_socket, client_socket);
	std::thread downstream_thread(downstream, server_socket, client_socket);

	if (upstream_thread.joinable())
		upstream_thread.join();
	if (downstream_thread.joinable())
		downstream_thread.join();

	server_socket.Close();
	client_socket.Close();
	std::cout << "- End connection: " << client_address << std::endl;
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
