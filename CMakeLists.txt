cmake_minimum_required(VERSION 3.20)

project(hydrogen)

set(CMAKE_CXX_STANDARD 20)

# Set the build type to Debug
set(CMAKE_BUILD_TYPE Debug)

add_executable(hydro src/main.cpp)
