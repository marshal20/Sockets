cmake_minimum_required(VERSION 3.1)

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

add_subdirectory(Network)
add_subdirectory(test)

add_subdirectory(DomainNameToIP)
add_subdirectory(SimpleTCPEcho)

