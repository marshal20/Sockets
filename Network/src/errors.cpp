#include "errors.hpp"
#include <string>
#include <exception>
#include <stdexcept>

namespace Error
{
	void runtime(const char* error, int code)
	{
		std::string msg = std::string(error) + std::string(", error: ") + std::to_string(code);
		throw std::runtime_error(msg.c_str());
	}

	void runtime(const char* error, const char* additional, int code)
	{
		std::string msg = std::string(error) + std::string(", ") + std::string(additional);
		runtime(msg.c_str(), code);
	}

}