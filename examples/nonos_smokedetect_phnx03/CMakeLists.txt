cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Release)
set(TARGET phnx03)
include("$ENV{PHNX_SDF}/tools/cmake/project.cmake")


set(PROJECT_SRCS
    "src/main.c"
)

set(PROJECT_DEFINITIONS "_LED_ALARM")

project(app)
