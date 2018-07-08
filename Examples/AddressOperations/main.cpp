#include <Network/Address.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
	std::cout << "IPv4 test: " << Address().setIP(IPv4({ 1,2,3,4 })) << std::endl;
	std::cout << "IPv6 test: " << Address().setIP(IPv6({ 1,2,3,4,5,6,7,8 })) << std::endl;
	std::cout << "IPv4 test str: " << Address().fromPresentation("1.2.3.4") << std::endl;
	std::cout << "IPv6 test str: " << Address().fromPresentation("1:2:3:4:5:6:7:8") << std::endl;
	std::cout << "localhost Address: " << Address::localhost() << std::endl;
	std::cout << "thishost Address: " << Address::thishost() << std::endl;
	std::cout << "Broadcast Address: " << Address::broadcast() << std::endl;
	std::cout << "localhost Address IPv6: " << Address::localhost(Protocol::IPv6) << std::endl;
	std::cout << "thishost Address IPv4: " << Address::thishost(Protocol::IPv6) << std::endl;
	std::cout << "google.com: " << Address::fromPresentation("google.com") << std::endl;
	std::cout << "example.com: " << Address::fromPresentation("example.com") << std::endl;

	return 0;
}
