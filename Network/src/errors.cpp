#include "errors.hpp"
#include <string>
#include <exception>
#include <stdexcept>
#include <string.h>
#include <errno.h>

#if not WIN32
#define strerror_s(b, s, c) strerror_r(c, b, s)
#endif

namespace Error
{
	std::string error_to_string(int errorcode)
	{
		char temporary_errorstr_buffer[256];
		strerror_s(temporary_errorstr_buffer, sizeof(temporary_errorstr_buffer), errorcode);
		std::string error_str = temporary_errorstr_buffer;
		return error_str;
	}

	void runtime(const char* error, int code)
	{
		std::string msg = std::string(error) + std::string(", ") + error_to_string(code)
			+ std::string(", error code: ") + std::to_string(code);
		throw std::runtime_error(msg.c_str());
	}

	void runtime(const char* error, const char* additional, int code)
	{
		std::string msg = std::string(error) + std::string(", ") + std::string(additional) + std::string(", error code: ") + std::to_string(code);
		throw std::runtime_error(msg.c_str());
	}

}
