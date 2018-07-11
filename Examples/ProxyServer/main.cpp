#include <Network/Address.hpp>
#include <Network/Socket.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <regex>

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

struct ConnectRequest
{
	bool valid;
	std::string connectStr;
	std::string domainName;
	std::string protocol;
};

ConnectRequest parseRequest(const std::string& req)
{
	//std::string firstLine = splitString(req, '\r')[0];
	std::regex Connectregex("(\\S+)\\s+(\\S+)\\s+(\\S+)");
	std::smatch matchs;
	std::regex_search(req, matchs, Connectregex);
	if (matchs.size() == 4)
		return { true, matchs[1], matchs[2] , matchs[3] };

	return { false, "", "", "" };
}

Address getAddressFromDomain(std::string domainName)
{
	auto addrportList = splitString(domainName, ':');
	return Address::fromPresentation(addrportList[0]).setPort(std::stoi(addrportList[1]));
}

std::string getHost(const std::string& request)
{
	std::string host = "";
	std::regex Hostregex("Host\\s*:\\s*(\\S+)");
	std::smatch matchs;
	

	auto reqstrs = splitString(request, '\n');
	for (const auto& s : reqstrs)
	{
		if (s.find("Host") != std::string::npos)
		{
			std::regex_search(request, matchs, Hostregex);
			if (matchs.size() == 2)
				host = matchs[1];
		}
			//host = splitString(s, ':')[1];
	}
	return host;
}

void connectCommand(Socket client_sock, char* buff, int currec)
{
	Address toConnect;
	Socket server_sock;

	ConnectRequest req = parseRequest(buff);

	if (!req.valid)
	{
		std::string res = req.protocol + " 403 forbidden\r\n\r\n";
		client_sock.send(res.c_str(), res.length());
		client_sock.close();
		return;
	}

	toConnect = getAddressFromDomain(req.domainName);
	server_sock.connect(toConnect);
	std::cout << "- Connected to address: " << toConnect << ", port:" << toConnect.getPort() << std::endl;

	std::string res = req.protocol + " 200 ok\r\n\r\n";
	client_sock.send(res.c_str(), res.length());
	std::cout << "proxy res: " << res << std::endl;

	int send;
	bool close = false;
	while (!close)
	{
		currec = client_sock.recv(buff, sizeof(buff));
		if (currec == 0) { close = true; continue; }
		std::cout << "- Recieved new_sock " << currec << " Bytes" << std::endl;

		//send = targetSock.send(buff, currec);
		send = server_sock.send(buff, currec);
		std::cout << "- sent targetSock " << send << " Bytes" << std::endl;

		currec = server_sock.recv(buff, sizeof(buff));
		if (currec == 0) close = true;
		std::cout << "- Recieved targetSock " << currec << " Bytes" << std::endl;
		std::cout << buff << std::endl;

		//send = new_sock.send(buff, currec);
		send = client_sock.send(buff, currec);
		std::cout << "- sent new_sock " << send << " Bytes" << std::endl;

		std::cout << "----- RequestServed -----\n\n";
	}

	server_sock.close();
}

void getCommand(Socket client_sock, char* buff, int currec)
{
	std::string website = getHost(buff);
	Socket server_sock;
	server_sock.connect(Address::fromPresentation(website).setPort(80));

	bool close = false;
	while (!close)
	{
		int send;
		if (currec == 0) { close = true; continue; }

		send = server_sock.send(buff, currec);
		std::cout << "- Sent to website " << send << " Bytes" << std::endl;

		currec = server_sock.recv(buff, sizeof(buff));
		if (currec == 0) close = true;
		std::cout << "- Recieved from website " << currec << " Bytes" << std::endl;

		send = client_sock.send(buff, currec);
		std::cout << "- Sent to client " << send << " Bytes" << std::endl;

		currec = client_sock.recv(buff, sizeof(buff));

		std::cout << "----- RequestServed -----\n";

		std::cout << "\n----- RequestStarted -----\n";
		std::cout << "- Recieved from client " << currec << " Bytes" << std::endl;
	}

	server_sock.close();
}

void serveClient(Socket client_sock) try
{
	char buff[16384*2];
	int currec;

	currec = client_sock.recv(buff, sizeof(buff));
	buff[currec] = 0;
	std::cout << "- Recieved " << currec << " Bytes, content: " << buff << std::endl << std::endl;

	ConnectRequest req = parseRequest(buff);
	if (req.connectStr == "CONNECT")
	{
		std::cout << "----------------- CONNECT" << std::endl;
		connectCommand(client_sock, buff, currec);
	}
	else
	{
		std::cout << "----------------- GET" << std::endl;
		getCommand(client_sock, buff, currec);
	}

	printf("- Info: connection closed, receved: %d bytes, sent: %d bytes\n",
		client_sock.getTotalrecv(), client_sock.getTotalsent());
}
catch (const std::exception& e)
{
	std::cout << "- Failed client: " << e.what() << std::endl;
	return;
}

int main(int argc, char* argv[]) try
{
	const char* port = "5000";
	if (argc == 2)
		port = argv[1];

	Protocol addrProtocol = Protocol::IPv4;
	if (argc > 2)
		if (argv[2][0] == '6')
			addrProtocol = Protocol::IPv6;

	Address localhostTarget = Address::thishost(addrProtocol).setPort(std::stoi(port)); // thishost:port
	Socket sock(Socket::Type::Stream, localhostTarget.getProtocol());

	sock.bind(localhostTarget);
	std::cout << "- Info: socket bound to " << localhostTarget << " port: " << port << std::endl;
	sock.listen();

	Socket client_sock; Address client_addr;
	while (client_sock = sock.accept(client_addr))
	{
		// new connection
		std::cout << "- Info: new connection, Address: " << client_addr << std::endl;

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
