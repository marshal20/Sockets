cmake_minimum_required(VERSION 3.1)

project(DomainNameToIP)

add_executable(DomainNameToIP main.cpp)

target_link_libraries(DomainNameToIP PRIVATE Network)