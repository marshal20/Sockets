cmake_minimum_required(VERSION 3.1)

project(DgramPacketsBroadcaster)

add_executable(DgramPacketsBroadcaster main.cpp)

target_link_libraries(DgramPacketsBroadcaster PRIVATE Network)