cmake_minimum_required(VERSION 3.1)

project(Test)

add_executable(Test main.cpp)

target_link_libraries(Test PRIVATE Network)