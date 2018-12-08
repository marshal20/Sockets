cmake_minimum_required(VERSION 3.1)

project(Proxy)

find_package( Threads REQUIRED )
add_executable(Proxy main.cpp)

target_link_libraries(Proxy PRIVATE Network Threads::Threads)
