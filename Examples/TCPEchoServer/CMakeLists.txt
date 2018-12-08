cmake_minimum_required(VERSION 3.1)

project(TCPEchoServer)

find_package( Threads REQUIRED )
add_executable(TCPEchoServer main.cpp)

target_link_libraries(TCPEchoServer PRIVATE Network Threads::Threads)
