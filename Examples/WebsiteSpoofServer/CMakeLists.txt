cmake_minimum_required(VERSION 3.1)

project(WebsiteSpoofServer)

add_executable(WebsiteSpoofServer main.cpp)

target_link_libraries(WebsiteSpoofServer PRIVATE Network)