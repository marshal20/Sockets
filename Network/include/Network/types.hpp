#pragma once
#include <string>

enum class Protocol
{
	IPv4 = 1,
	IPv6
};

inline std::string ProtocolToString(Protocol value)
{
	return (value == Protocol::IPv4) ? std::string("IPv4") : std::string("IPv6");
}