cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Release)
set(TARGET phnx02)
include("$ENV{PHNX_SDF}/tools/cmake/project.cmake")

file(GLOB PROJECT_SRCS "src/*.c")

project(app)
