cmake_minimum_required(VERSION 3.1)

project(Test2)

add_executable(Test2 main.cpp)

target_link_libraries(Test2 PRIVATE Network)