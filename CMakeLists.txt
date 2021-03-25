cmake_minimum_required(VERSION 3.17)
project(raytracing)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS -fopenmp)

add_executable(raytracing main.cc image.cc type.cc objects.cc light.cc camera.cc scene.cc)