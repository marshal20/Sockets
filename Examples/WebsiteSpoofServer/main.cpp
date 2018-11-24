#include <Network/Address.hpp>
#include <Network/Socket.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <tuple>

using namespace nt; // Not recommended.

// global constants
const char* port = "8080";
const char* website = "example.com";

std::vector<std::string> splitString(std::string strToSplit, char del)
{
	std::vector<std::string> temp;
	std::stringstream strStream;
	strStream << strToSplit;
	std::string chunk;
	while (std::getline(strStream, chunk, del))
	{
		temp.push_back(chunk);
	}

	return temp;
}

std::string getHost(const std::string& request)
{
	std::string host = "";
	auto reqstrs = splitString(request, '\n');
	for (const auto& s : reqstrs)
	{
		if (s.find("Host") != std::string::npos)
			host = splitString(s, ':')[1];
	}
	return host;
}

void replaceHost(std::string& request, const std::string& new_host)
{
	auto reqstrs = splitString(request, '\n');
	for (auto& s : reqstrs)
	{
		if (s.find("Host") != std::string::npos)
		{
			s = std::string("Host: ") + new_host;
			break;
		}
	}
	request = "";
	for (auto& s : reqstrs)
		request += s + "\n";
	request += "\r\n";
}

void serveClient(Socket client_sock)
{
	char buff[8192];
	int currec;

	Socket server_sock;
	server_sock.connect({ Address::fromPresentation(website), 80 });

	bool close = false;
	while (!close)
	{
		int send;

		currec = client_sock.recv(buff, sizeof(buff));
		if (currec == 0) { close = true; continue; }
		std::cout << "\n----- RequestStarted -----\n";
		std::cout << "- Recieved from client " << currec << " Bytes" << std::endl;

		buff[currec] = 0;
		std::string request = buff;

		std::string myHost = getHost(request);
		replaceHost(request, website);

		send = server_sock.send(request.c_str(), request.length());
		std::cout << "- Sent to website " << send << " Bytes" << std::endl;

		currec = server_sock.recv(buff, sizeof(buff));
		if (currec == 0) close = true;
		std::cout << "- Recieved from website " << currec << " Bytes" << std::endl;

		send = client_sock.send(buff, currec);
		std::cout << "- Sent to client " << send << " Bytes" << std::endl;

		std::cout << "----- RequestServed -----\n";
	}

	server_sock.close();
}

int main(int argc, char* argv[]) try
{
	if (argc > 1)
		port = argv[1];

	if (argc > 2)
		website = argv[2];

	Address localhostTarget = Address::thishost(Family::IPv4); // thishost:port
	Socket sock(Socket::Type::Stream, localhostTarget.getFamily());

	sock.bind({ localhostTarget, (unsigned short)std::stoi(port) });
	std::cout << "- Info: socket bound to " << localhostTarget << " port: " << port << std::endl;
	std::cout << "- Server will spoof: " << website << std::endl;
	sock.listen();

	while (true)
	{
		Socket client_sock; EndPoint new_addr;
		std::tie(client_sock, new_addr) = sock.accept();
		// new connection
		std::cout << "- Info: new connection, Address: " 
			<< new_addr.address << ":" << new_addr.port << std::endl;
		serveClient(client_sock);
	}

	sock.close();
	return 0;
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
	return 1;
}
