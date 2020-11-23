# Target system name
include(CMakeForceCompiler)
set(CMAKE_SYSTEM_NAME Generic)

# set(COMPILER_PREFIX "arm-none-eabi-")
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
    "-fstrict-volatile-bitfields"
    "-Wno-old-style-declaration"
    "-ggdb" "-Os"
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
    "--specs=nosys.specs"
    # "--specs=nano.specs"
    "-Wl,--gc-sections"
)

if ("${CPU}" STREQUAL "cortex-m0plus")
    set(CPU_FLAGS "-mcpu=cortex-m0plus -mthumb -mfloat-abi=soft")
else()
    # set(CPU_FLAGS "-mcpu=cortex-m3 -march=armv7-m -mthumb -mthumb-interwork -mapcs-frame -mabi=aapcs -mfloat-abi=softfp -mlittle-endian")
    set(CPU_FLAGS "-mcpu=cortex-m3 -mthumb -mfloat-abi=softfp")
endif()

set(CMAKE_C_FLAGS ${CPU_FLAGS})
set(CMAKE_ASM_FLAGS
    "${CPU_FLAGS} -Os -ggdb  -ffunction-sections -fdata-sections"
)

foreach(elm ${cflags_list})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${elm}")
endforeach()
