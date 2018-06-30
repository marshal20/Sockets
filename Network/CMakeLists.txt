cmake_minimum_required (VERSION 3.1)

set(SRC src/*.cpp
		src/*.hpp
		include/Network/*.hpp)

if(WIND32)
	set(SRC $(SRC) 
			src/WIN32/*.cpp 
			src/WIN32/*.hpp)
elseif(UNIX)
	set(SRC $(SRC) 
			src/UNIX/*.cpp 
			src/UNIX/*.hpp)

add_library(Network $(SRC))

target_include_directories(Network
							PRIVATE src
							PUBLIC inlcude)

