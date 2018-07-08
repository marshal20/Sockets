cmake_minimum_required(VERSION 3.1)

project(TCPEchoServer)

add_executable(TCPEchoServer main.cpp)

target_link_libraries(TCPEchoServer PRIVATE Network)