#include <Network/Address.hpp>
#include <Network/Socket.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>

const char* HTTP_200_OK = " 200 OK";
const char* HTTP_403_FORBIDDEN= "\nHTTP/1.1 403 FORBIDDEN";

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

/*Address getConnectAddress(std::string line)
{
	std::string addrAndport = splitString(line, ' ')[1];
	auto addrportList = splitString(addrAndport, ':');
	return Address::fromPresentation(addrportList[0]).setPort(std::stoi(addrportList[1]));
}

std::string getFirstLine(const char* buff)
{
	std::stringstream buffStream;
	buffStream << buff;
	std::string firstLine;
	std::getline(buffStream, firstLine);
	return firstLine;
}*/

struct ConnectRequest
{
	std::string connectStr;
	std::string domainName;
	std::string protocol;
};

ConnectRequest parseRequest(std::string req)
{
	auto firstLineParts = splitString(splitString(req, '\r')[0], ' ');
	return { firstLineParts[0], firstLineParts[1], firstLineParts[2] };
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

void connectionRecieve(Socket new_sock, Socket targetSock, char* buffr, int& currecr)
{
	while (currecr = new_sock.recv(buffr, sizeof(buffr)) != 0)
	{
		std::cout << "- Recieved new_sock " << currecr << " Bytes" << std::endl;
		int send = sendall(targetSock, buffr, currecr);
		std::cout << "- sent targetSock " << send << " Bytes" << std::endl;
	}
}

void connectionSend(Socket new_sock, Socket targetSock, char* buffs, int& currecs)
{
	while (currecs = targetSock.recv(buffs, sizeof(buffs)) != 0)
	{
		std::cout << "- Recieved targetSock " << currecs << " Bytes" << std::endl;
		int send = sendall(new_sock, buffs, currecs);
		std::cout << "- sent new_sock " << send << " Bytes" << std::endl;
	}
}

int serveConnection(Socket new_sock)
{
	char buff[9000];
	int currec;

	Socket googleServer;
	googleServer.connect(Address::fromPresentation("bigclive.com").setPort(80));

	int send;
	while ((currec = new_sock.recv(buff, sizeof(buff))) != 0)
	{
		buff[currec] = 0;
		std::cout << "- Recieved new_sock " << currec << " Bytes" << std::endl;

		

		//send = targetSock.send(buff, currec);
		auto reqstrs = splitString(buff, '\n');
		for (auto& s : reqstrs)
		{
			if (s.find("Host") != std::string::npos)
				s = "Host: bigclive.com";
		}
		std::string googleReq = "";
		for (auto& s : reqstrs)
			googleReq += s + "\n";
		googleReq += "\r\n";
		send = googleServer.send(googleReq.c_str(), strlen(googleReq.c_str()));
		std::cout << "- sent targetSock " << send << " Bytes" << std::endl;
		std::cout << std::endl << googleReq << std::endl;
		currec = googleServer.recv(buff, sizeof(buff));
		buff[currec] = 0;
		std::cout << "- Recieved targetSock " << currec << " Bytes" << std::endl;

		//send = new_sock.send(buff, currec);
		send = sendall(new_sock, buff, currec);
		std::cout << "- sent new_sock " << send << " Bytes" << std::endl;

		std::cout << std::endl << buff << std::endl;

		std::cout << "----- RequestServed -----\n\n";
	}

	return 0;

	//////////////////////////////



	currec = new_sock.recv(buff, sizeof(buff));
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
	std::cout << new_sock.send(res.c_str(), strlen(res.c_str())) << std::endl;
	std::cout << "res: " << res << std::endl;

	//char buffr[9000];
	//int currecr;
	//char buffs[9000];
	//int currecs;
	//std::thread recv(connectionRecieve, new_sock, targetSock, buffr, currecr);

	//std::thread send(connectionSend, new_sock, targetSock, buffs, currecs);
	//send.join();

	//int send;
	while ((currec = new_sock.recv(buff, sizeof(buff))) != 0)
	{
		std::cout << "- Recieved new_sock " << currec << " Bytes" << std::endl;

		//send = targetSock.send(buff, currec);
		send = sendall(targetSock, buff, currec);
		std::cout << "- sent targetSock " << send << " Bytes" << std::endl;
		currec = targetSock.recv(buff, sizeof(buff));
		std::cout << "- Recieved targetSock " << currec << " Bytes" << std::endl;
		std::cout << buff << std::endl;

		//send = new_sock.send(buff, currec);
		send = sendall(new_sock, buff, currec);
		std::cout << "- sent new_sock " << send << " Bytes" << std::endl;

		std::cout << "----- RequestServed -----\n\n";
	}

	printf("- Info: connection closed, receved: %d bytes, sent: %d bytes\n",
		new_sock.getTotalrecv(), new_sock.getTotalsent());
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

	Socket new_sock; Address new_addr;
	while (new_sock = sock.accept(new_addr))
	{
		// new connection
		std::cout << "- Info: new connection, Address: " << new_addr << std::endl;

		serveConnection(new_sock);
	}

	sock.close();
	return 0;
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
	return 1;
}
