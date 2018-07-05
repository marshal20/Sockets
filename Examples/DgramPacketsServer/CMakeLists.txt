cmake_minimum_required(VERSION 3.1)

project(DgramPacketsServer)

add_executable(DgramPacketsServer main.cpp)

target_link_libraries(DgramPacketsServer PRIVATE Network)