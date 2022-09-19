cmake_minimum_required(VERSION 3.23)
project(Pathfinding)

set(CMAKE_CXX_STANDARD 23)

set(PROJECT_HEADERS
        tinyxml2.h
        olcPixelGameEngine.h
        )
set(PROJECT_SOURCES
        main.cpp
        tinyxml2.cpp
        )

add_executable(Pathfinding ${PROJECT_SOURCES} ${PROJECT_HEADERS})
