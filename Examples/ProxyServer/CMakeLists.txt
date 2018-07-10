cmake_minimum_required(VERSION 3.1)

project(ProxyServer)

add_executable(ProxyServer main.cpp)

target_link_libraries(ProxyServer PRIVATE Network)