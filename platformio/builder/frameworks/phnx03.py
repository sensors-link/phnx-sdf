from os.path import isdir, join

from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

board = env.BoardConfig()

# FRAMEWORK_DIR = env.PioPlatform().get_package_dir("framework-phoenix03-sdk")
FRAMEWORK_DIR = join(env.get("ENV")["PHNX_SDF"], "components", "phnx03")
assert FRAMEWORK_DIR and isdir(FRAMEWORK_DIR)

env.Replace(
    AR="riscv-none-embed-gcc-ar",
    AS="riscv-none-embed-as",
    CC="riscv-none-embed-gcc",
    GDB="riscv-none-embed-gdb",
    CXX="riscv-none-embed-g++",
    OBJCOPY="riscv-none-embed-objcopy",
    RANLIB="riscv-none-embed-gcc-ranlib",
    SIZETOOL="riscv-none-embed-size",

    # LDSCRIPT_PATH = join(FRAMEWORK_DIR, "ldscripts", "flash.ld"),
    LDSCRIPT_PATH = join(FRAMEWORK_DIR, "ldscripts", "GD32VF103x8.lds"),
    
    UPLOADERFLAGS=[
        "-f", "interface/ftdi/sipeed-rv-debugger.cfg",
        "-f", "target/gd32vf103.cfg",
        "-c", "adapter_khz 8000",
        "-c", "init; halt;",
        "-c", "flash protect 0 0 last off; program {$SOURCE} verify; mww 0xe004200c 0x4b5a6978; mww 0xe0042008 0x01; resume; exit 0;"
    ],
)

MCUFLAGS = ["-march=rv32imac", "-mabi=ilp32", "-mcmodel=medany", "-msmall-data-limit=8"]

env.Append(
    ASFLAGS = MCUFLAGS,
    CCFLAGS = MCUFLAGS,
    LINKFLAGS = MCUFLAGS,
    CPPDEFINES = [],

    CPPPATH = [
        join(FRAMEWORK_DIR, "include"),
        join(FRAMEWORK_DIR, "drivers"),
        join(FRAMEWORK_DIR, "peripheral")        
    ],

    LIBS = [
        env.BuildLibrary( join("$BUILD_DIR", "phnx03"), 
            FRAMEWORK_DIR, [
                "+<drivers/*.c>",
                "+<peripheral/*.c>",
                "+<startup/*.S>", "+<startup/handlers.c>", "+<startup/init.c>", "+<startup/retarget.c>", "+<startup/system_gd32vf103.c>"
            ]
        )
    ]

)