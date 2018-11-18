#pragma once
#include <string>

enum class Family
{
	IPv4 = 1,
	IPv6
};

inline std::string ProtocolToString(Family value)
{
	return (value == Family::IPv4) ? std::string("IPv4") : std::string("IPv6");
}