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

ConnectRequest parseRequest(std::string req)
{
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

int sendall(Socket& s, char *buf, int len)
{
	int total = 0; // how many bytes we've sent
	int bytesleft = len; // how many we have left to send
	int n;
	while (total < len) {
		n = s.send(buf + total, bytesleft);
		if (n == -1) { break; }
		total += n;
		bytesleft -= n;
	}
	return total; // return number actually sent here
	return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}

int serveClient(Socket client_sock)
{
	char buff[8192];
	int currec;

	currec = client_sock.recv(buff, sizeof(buff));
	buff[currec] = 0;
	std::cout << "- Recieved " << currec << " Bytes, content: " << buff << std::endl;

	ConnectRequest req;
	Socket targetSock;
	try
	{
		req = parseRequest(buff);
		Address toConnect = getAddressFromDomain(req.domainName);
		std::cout << "- Connect address: " << toConnect << ", port:" << toConnect.getPort() << std::endl;
		targetSock.connect(toConnect);
	}
	catch (const std::exception& e)
	{
		std::string res = req.protocol + " 403 forbidden\r\n\r\n";
		return 0;
	}


	std::string res = req.protocol + " 200 ok\r\n\r\n";
	std::cout << client_sock.send(res.c_str(), strlen(res.c_str())) << std::endl;
	std::cout << "res: " << res << std::endl;



	int send;
	while ((currec = client_sock.recv(buff, sizeof(buff))) != 0)
	{
		std::cout << "- Recieved new_sock " << currec << " Bytes" << std::endl;

		//send = targetSock.send(buff, currec);
		send = sendall(targetSock, buff, currec);
		std::cout << "- sent targetSock " << send << " Bytes" << std::endl;
		currec = targetSock.recv(buff, sizeof(buff));
		std::cout << "- Recieved targetSock " << currec << " Bytes" << std::endl;
		std::cout << buff << std::endl;

		//send = new_sock.send(buff, currec);
		send = sendall(client_sock, buff, currec);
		std::cout << "- sent new_sock " << send << " Bytes" << std::endl;

		std::cout << "----- RequestServed -----\n\n";
	}

	printf("- Info: connection closed, receved: %d bytes, sent: %d bytes\n",
		client_sock.getTotalrecv(), client_sock.getTotalsent());
}

int main(int argc, char* argv[]) try
{
	const char* port = "80";
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
