#pragma once

#if WIN32
#include "win32/sockImpl.hpp"
#else
#include "unix/sockImpl.hpp"
#endif