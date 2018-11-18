#include "Network/Address.hpp"
#include "sockImpl.hpp"

void create_sockaddr_from_address(const Address& address, unsigned short port, sockaddr_storage* sockaddr);

void create_address_from_sockaddr(Address& address, unsigned short& port, const sockaddr_storage* sockaddr);
Address create_address_from_ipv4addr(int val);
Address create_address_from_ipv6addr(const struct in6_addr* in6addr);
