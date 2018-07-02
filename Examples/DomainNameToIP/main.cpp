#include <iostream>
#include <Network/Address.hpp>

int main(int argc, char* argv[]) try
{
	if (argc != 2)
	{
		std::cout << "- Usage " << argv[0] << " <domain name>" << std::endl;
		return 1;
	}
	char* domainName = argv[1];
	std::cout << "Addresses for " << domainName << ":" << std::endl;
	std::vector<Address> addrs = Address::fromPresentationAll(domainName);
	for (auto addr : addrs)
		std::cout << ProtocolToString(addr.getProtocol()) << ": " << addr.getPresentation() << std::endl;

	return 0;
}
catch (std::exception& e)
{
	std::cout << e.what() << std::endl;
	std::cin.get();
}
