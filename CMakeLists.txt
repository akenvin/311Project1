# Updated on May 16, 2017

cmake_minimum_required(VERSION 2.8)

project(UnixDomainSocketTest CXX)

add_executable(server server.cpp)
add_executable(client client.cpp)
