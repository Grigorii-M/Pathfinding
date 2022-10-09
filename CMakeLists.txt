cmake_minimum_required(VERSION 3.4)
project(Pathfinding)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)

set(PROJECT_HEADERS
        src/tinyxml2.h
        src/olcPixelGameEngine.h
        src/Grid.h
        src/Pathfinder.h
        )

set(PROJECT_SOURCES
        src/main.cpp
        src/tinyxml2.cpp
        src/Grid.cpp
        src/Pathfinder.cpp
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
