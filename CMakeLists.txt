cmake_minimum_required(VERSION 3.27)
project(untitled1)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled1 main.cpp
        Headers/Menu.h
        Sources/Menu.cpp
        Headers/Utilities.h
        Sources/Utilities.cpp
        Sources/SimulatedAnnealing.cpp
        Headers/SimulatedAnnealing.h
        Headers/Greedy.h
        Sources/Greedy.cpp)
