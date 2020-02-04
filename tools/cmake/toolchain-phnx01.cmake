# Target system name
include(CMakeForceCompiler)
set(CMAKE_SYSTEM_NAME Generic)

set(COMPILER_PREFIX "arm-none-eabi")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Compilers and utilities
find_program(CMAKE_C_COMPILER NAMES ${COMPILER_PREFIX}-gcc)
find_program(CMAKE_ASM_COMPILER NAMES ${COMPILER_PREFIX}-gcc)
find_program(CMAKE_OBJCOPY NAMES ${COMPILER_PREFIX}-objcopy)
find_program(CMAKE_SIZE NAMES ${COMPILER_PREFIX}-size)
find_program(CMAKE_DEBUGGER NAMES ${COMPILER_PREFIX}-gdb)

# Compilers flags
set(cflags_list
    # Default compiler configuration
    "-ffunction-sections"
    "-fdata-sections"
    "-fstrict-volatile-bitfields"
    "-Wno-old-style-declaration"
    "-ggdb"
    "-Os"
    "-nostartfiles"

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

    # Linker flags
    "--specs=nano.specs"
    "-Wl,--gc-sections"
)

set(CPU_FLAGS "-mcpu=cortex-m0plus -mthumb -mfloat-abi=soft")
set(CMAKE_C_FLAGS ${CPU_FLAGS})
set(CMAKE_ASM_FLAGS ${CPU_FLAGS})

foreach(elm ${cflags_list})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${elm}")
endforeach()
