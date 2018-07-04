cmake_minimum_required(VERSION 3.1)

project(AddressOperations)

add_executable(AddressOperations main.cpp)

target_link_libraries(AddressOperations PRIVATE Network)
