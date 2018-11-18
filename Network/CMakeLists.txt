cmake_minimum_required (VERSION 3.1)

set(SRC 
		src/Address.cpp 
		include/Network/Address.hpp
		src/Socket.cpp 
		include/Network/Socket.hpp
		src/sockImpl.hpp
		src/util.cpp
		src/util.hpp
		src/errors.cpp
		src/errors.hpp
		)

if(WIN32)
	set(SRC ${SRC} 
			src/win32/sockImpl.cpp 
			src/win32/sockImpl.hpp
			)
else()
	set(SRC ${SRC} 
			src/unix/sockImpl.cpp 
			src/unix/sockImpl.hpp
			)
endif()

add_library(Network STATIC ${SRC})

target_include_directories(Network
  PRIVATE src
  PUBLIC include
)

if(WIN32)
	target_link_libraries(Network PRIVATE ws2_32)
endif()
