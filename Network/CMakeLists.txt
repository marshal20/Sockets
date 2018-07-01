cmake_minimum_required (VERSION 3.1)

set(SRC 
		src/Address.cpp 
		include/Network/Address.hpp
		 
		)

if(WIN32)
	set(SRC ${SRC} 
			src/win32/winsockinit.cpp 
			src/win32/winsockinit.hpp
			)
else()
	set(SRC ${SRC} 
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
