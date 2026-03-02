cmake_minimum_required(VERSION 3.28)

project(mqtt-lab)

find_package(Boost REQUIRED COMPONENTS json)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(solutions)