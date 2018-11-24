cmake_minimum_required(VERSION 3.1)

project(Proxy)

add_executable(Proxy main.cpp)

target_link_libraries(Proxy PRIVATE Network)