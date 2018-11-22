cmake_minimum_required(VERSION 3.1)

project(TCPEchoServer-multithread)

add_executable(TCPEchoServer-multithread main.cpp)

target_link_libraries(TCPEchoServer-multithread PRIVATE Network)