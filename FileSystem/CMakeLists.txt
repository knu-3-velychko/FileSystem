# cmake_minimum_required(VERSION <specify CMake version here>)
project(FileSystem)

set(CMAKE_CXX_STANDARD 17)

add_executable(FileSystem main.cpp Shell.cpp Shell.h IOSystem.cpp IOSystem.h FileSystem.cpp FileSystem.h)