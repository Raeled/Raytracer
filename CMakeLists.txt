cmake_minimum_required (VERSION 2.8.11)
project (raytracer)

set(CMAKE_CXX_STANDARD 14)

configure_file(monkey.obj monkey.obj COPYONLY)
configure_file(plane.obj plane.obj COPYONLY)

include_directories(
    src
    lib/stb
    )

add_executable (raytracer
        lib/stb/stb_implementation.c
        src/ObjFile.cpp
        src/main.cpp)