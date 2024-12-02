cmake_minimum_required(VERSION 3.29)
project(Muzik)

set(CMAKE_CXX_STANDARD 20)
find_package(raylib)

add_executable(Muzik main.cpp)
target_link_libraries(Muzik PRIVATE raylib)

