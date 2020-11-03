# Target system name
include(CMakeForceCompiler)
set(CMAKE_SYSTEM_NAME Generic)

set(COMPILER_PREFIX "riscv-none-embed")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Compilers and utilities
find_program(CMAKE_C_COMPILER NAMES ${COMPILER_PREFIX}-gcc)
find_program(CMAKE_ASM_COMPILER NAMES ${COMPILER_PREFIX}-gcc)
find_program(CMAKE_ASM_COMPILER NAMES ${COMPILER_PREFIX}-gcc)
find_program(CMAKE_OBJCOPY NAMES ${COMPILER_PREFIX}-objcopy)
find_program(CMAKE_SIZE NAMES ${COMPILER_PREFIX}-size)
find_program(CMAKE_DEBUGGER NAMES ${COMPILER_PREFIX}-gdb)
find_program(CMAKE_QEMU NAMES qemu-system-riscv32)

# Compilers flags
set(cflags_list
    # Default compiler configuration
    "-ffunction-sections"
    "-fdata-sections"
    "-fstrict-volatile-bitfields"
    "-Wno-old-style-declaration"
    "-ggdb"
    "-Os"
    # "-fPIE" "-pie"
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

set(CPU_FLAGS "-march=rv32emac -mabi=ilp32e -msmall-data-limit=8")

set(CMAKE_C_FLAGS ${CPU_FLAGS})
set(CMAKE_ASM_FLAGS
    "${CPU_FLAGS} -Os -ggdb  -ffunction-sections -fdata-sections"
)

foreach(elm ${cflags_list})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${elm}")
endforeach()
