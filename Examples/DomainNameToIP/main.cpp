#include <Network/Network.hpp>
#include <iostream>

using namespace nt; // Not recommended.

int main(int argc, char* argv[]) try
{
	if (argc != 2)
	{
		std::cout << "- Usage " << argv[0] << " <domain name>" << std::endl;
		return 1;
	}
	char* domainName = argv[1];
	std::vector<Address> addrs = Address::FromPresentationAll(domainName);
	std::cout << "Addresses(" << addrs.size() << ") for " << domainName << ":" << std::endl;
	for (auto addr : addrs)
		std::cout << ProtocolToString(addr.GetFamily()) << ": " << addr.GetPresentation() << std::endl;

	return 0;
}
catch (std::exception& e)
{
	std::cout << "- Error:" << std::endl << e.what() << std::endl;
	return 1;
}
