#include <Network/Network.hpp>
#include <iostream>
#include <string>

using namespace nt; // Not recommended.

int main(int argc, char* argv[]) try
{
	std::cout << "IPv4 test: " << Address(IPv4({ 1,2,3,4 })) << std::endl;
	std::cout << "IPv6 test: " << Address(IPv6({ 1,2,3,4,5,6,7,8 })) << std::endl;
	std::cout << "IPv4 test str: " << Address("1.2.3.4") << std::endl;
	std::cout << "IPv6 test str: " << Address("1:2:3:4:5:6:7:8") << std::endl;
	std::cout << "localhost Address: " << Address::Localhost() << std::endl;
	std::cout << "thishost Address: " << Address::Thishost() << std::endl;
	std::cout << "Broadcast Address: " << Address::Broadcast() << std::endl;
	std::cout << "localhost Address IPv6: " << Address::Localhost(Family::IPv6) << std::endl;
	std::cout << "thishost Address IPv4: " << Address::Thishost(Family::IPv6) << std::endl;
	std::cout << "google.com: " << Address("google.com") << std::endl;
	std::cout << "example.com: " << Address("example.com") << std::endl;

	return 0;
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
	return 1;
}
