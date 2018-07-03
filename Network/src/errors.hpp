#pragma once

namespace Error
{
	extern void runtime(const char* error, int code = 0);
	extern void runtime(const char* error, const char* additional, int code = 0);
}