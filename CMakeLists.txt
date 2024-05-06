cmake_minimum_required(VERSION 3.27)
project(hw2)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw2 main.cpp
        architecture/Stack.cpp
        architecture/Stack.h
        architecture/Registers.cpp
        architecture/Registers.h
        architecture/Architecture.h
        architecture/Architecture.cpp
        architecture/Parser.h
        architecture/Parser.cpp
        architecture/Executor.cpp)

set(CMAKE_EXE_LINKER_FLAGS "-static")