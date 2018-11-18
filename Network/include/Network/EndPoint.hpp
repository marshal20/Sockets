#pragma once
#include <Network/Address.hpp>

/*
A pair of Address and port is called a socket address or endpoint.
*/

struct EndPoint
{
	Address address;
	unsigned short port;

	/*inline EndPoint(Address addr_, unsigned short port_) :
		address(addr_), port(port_)
	{}*/
};