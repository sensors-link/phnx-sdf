# Target system name
include(CMakeForceCompiler)
set(CMAKE_SYSTEM_NAME Generic)

# set(COMPILER_PREFIX "riscv-none-embed-")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Compilers and utilities
find_program(CMAKE_C_COMPILER NAMES ${COMPILER_PREFIX}gcc)
find_program(CMAKE_CXX_COMPILER NAMES ${COMPILER_PREFIX}g++)
find_program(CMAKE_ASM_COMPILER NAMES ${COMPILER_PREFIX}gcc)
find_program(CMAKE_OBJCOPY NAMES ${COMPILER_PREFIX}objcopy)
find_program(CMAKE_SIZE NAMES ${COMPILER_PREFIX}size)
find_program(CMAKE_DEBUGGER NAMES ${COMPILER_PREFIX}gdb)

# Compilers flags
set(cflags_list
    # Default compiler configuration
    "-ffunction-sections"
    "-fdata-sections"
    "-std=gnu11"
    "-fmessage-length=0"
    "-fsigned-char"
    "-fno-common"

    # Error & Warning
    "-Wall"
    "-Werror=all"
    "-Wno-error=unused-function"
    "-Wno-error=unused-but-set-variable"
    "-Wno-error=unused-variable"
    "-Wno-error=deprecated-declarations"
    "-Wextra"
    "-Wno-unused-parameter"
    "-Wno-sign-compare"

    "-fprofile-arcs"
    "-ftest-coverage"
    "-ggdb"
)

foreach(elm ${cflags_list})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${elm}")
endforeach()

set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_C_FLAGS} -Wl,--gc-sections")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# MINGW 目标可以支持C++
set(TARGET_ENABLE_CXX "1")
