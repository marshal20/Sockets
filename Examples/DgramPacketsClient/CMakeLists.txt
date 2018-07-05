cmake_minimum_required(VERSION 3.1)

project(DgramPacketsClient)

add_executable(DgramPacketsClient main.cpp)

target_link_libraries(DgramPacketsClient PRIVATE Network)