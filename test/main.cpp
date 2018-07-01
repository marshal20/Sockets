#include <iostream>
#include <Network/Address.hpp>

int main()
{
	std::vector<Address> addrs = Address::fromPresentationAll("google.com");
	for (auto addr : addrs)
		std::cout << addr.getPresentation() << std::endl;

	std::cin.get();

	return 0;
}
