#cmake_minimum_required(VERSION 3.27)
#project(untitled)
#
#set(CMAKE_CXX_STANDARD 23)
#
#add_executable(untitled main.cpp
#        Position.cpp
#        Position.h
#        Helper.cpp
#        Helper.h
#        MoveGenerator.cpp
#        MoveGenerator.h)

cmake_minimum_required(VERSION 3.27)
project(untitled)

set(CMAKE_CXX_STANDARD 23)

add_library(chess_lib
        Position.cpp
        Position.h
        Helper.cpp
        Helper.h
        MoveGenerator.cpp
        MoveGenerator.h
        Engine.cpp
        Engine.h
)

add_library(chess::lib ALIAS chess_lib)

add_executable(untitled main.cpp)
target_link_libraries(untitled PRIVATE chess::lib)

#testing

option(BUILD_TESTS "Build unit tests." ON)
if(BUILD_TESTS)
    enable_testing()
#    find_package(GTest REQUIRED)
#    include(GoogleTest)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)


    message(STATUS "Building tests...")

    add_executable(tests
            tests/tests.cpp
    )

    target_link_libraries(tests
            GTest::gmock
            GTest::gtest
            chess_lib #chess::lib?
    )
    gtest_discover_tests(tests)
endif()