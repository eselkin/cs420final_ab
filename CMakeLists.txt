cmake_minimum_required(VERSION 3.6)
project(cs420final_ab)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)
add_executable(${PROJECT_NAME} "main.cpp"
        libraries/testing_library.h
        libraries/timing_library.h
        libraries/extras_library.h
        libraries/ab_library.h
        libraries/state.h)
