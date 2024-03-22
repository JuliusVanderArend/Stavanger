cmake_minimum_required(VERSION 3.27)
project(untitled)

set(CMAKE_CXX_STANDARD 23)

add_executable(untitled main.cpp
        Position.cpp
        Position.h
        Helper.cpp
        Helper.h
        MoveGenerator.cpp
        MoveGenerator.h)
