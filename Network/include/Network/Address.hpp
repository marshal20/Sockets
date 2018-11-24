#pragma once
#include <Network/types.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <utility>

struct sockaddr_storage;

namespace nt
{

	struct IPv4 {
		uint8_t a, b, c, d;
	};

	struct IPv6 {
		uint16_t a, b, c, d, e, f, g, h;
	};

	struct Addr
	{
		Family family;
		union
		{
			IPv4 v4;
			IPv6 v6;
		};
	};

	class Address
	{
	public:
		Address();
		Address(const IPv4& value);
		Address(const IPv6& value);
		Address(const std::string& presentation);
		~Address();

		friend std::ostream& operator<<(std::ostream& os, const Address& addr);

		Family GetFamily() const;
		std::string GetPresentation() const;

		static std::vector<Address> FromPresentationAll(const std::string& rep);

		static Address Localhost(Family family = Family::IPv4);
		static Address Thishost(Family family = Family::IPv4);
		static Address Broadcast();

	private:
		friend class Socket;
		friend void create_sockaddr_from_address(const Address&, unsigned short, sockaddr_storage*);

	private:
		Addr m_addr;
		bool m_valid = false;
	};

}

