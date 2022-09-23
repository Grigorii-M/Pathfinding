cmake_minimum_required(VERSION 3.23)
project(Pathfinding)

set(CMAKE_CXX_STANDARD 23)

set(PROJECT_HEADERS
        tinyxml2.h
        olcPixelGameEngine.h
        Grid.h
        Pathfinder.h
        )

set(PROJECT_SOURCES
        main.cpp
        tinyxml2.cpp
        Grid.cpp
        Pathfinder.cpp
        )

add_executable(Pathfinding ${PROJECT_SOURCES} ${PROJECT_HEADERS})

if(MSVC)
elseif(WIN32)
    set(LIBRARIES user32 gdi32 opengl32 gdiplus Shlwapi dwmapi stdc++fs)
    target_link_libraries(Pathfinding ${LIBRARIES})
elseif(UNIX)
    set(LIBRARIES X11 GL pthread png stdc++fs)
    target_link_libraries(Pathfinding ${LIBRARIES})
endif()
