cmake_minimum_required(VERSION 3.17)
project(raytracing)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS -fopenmp)

add_executable(raytracing
    src/main.cc
    src/image.cc
    src/type.cc
    src/object.cc
    src/camera.cc
    src/scene.cc
    src/light.cc)
