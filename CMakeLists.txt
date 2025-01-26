cmake_minimum_required(VERSION 3.27)
project(untitled1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "-static")
MATH(EXPR stack_size "16 * 1024 * 1024") # 16 Mb
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")

add_executable(untitled1 main.cpp
        Headers/Menu.h
        Sources/Menu.cpp
        Headers/Utilities.h
        Sources/Utilities.cpp
        Sources/SimulatedAnnealing.cpp
        Headers/SimulatedAnnealing.h
        Headers/Greedy.h
        Sources/Greedy.cpp
        Sources/TabuSearch.cpp
        Headers/TabuSearch.h
        Headers/Algorithm.h
        Sources/Algorithm.cpp
        Sources/AntColony.cpp
        Headers/AntColony.h)
