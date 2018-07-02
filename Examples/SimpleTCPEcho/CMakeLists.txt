cmake_minimum_required(VERSION 3.1)

project(SimpleTCPEcho)

add_executable(SimpleTCPEcho main.cpp)

target_link_libraries(SimpleTCPEcho PRIVATE Network)