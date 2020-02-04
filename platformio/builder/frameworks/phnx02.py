from os.path import isdir, join

from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

board = env.BoardConfig()

# FRAMEWORK_DIR = env.PioPlatform().get_package_dir("framework-phoenix02-sdk")
FRAMEWORK_DIR = join(env.get("ENV")["PHNX_SDF"], "components", "phnx02")
assert FRAMEWORK_DIR and isdir(FRAMEWORK_DIR)

env.Replace(
    AR="arm-none-eabi-gcc-ar",
    AS="arm-none-eabi-as",
    CC="arm-none-eabi-gcc",
    GDB="arm-none-eabi-gdb",
    CXX="arm-none-eabi-g++",
    OBJCOPY="arm-none-eabi-objcopy",
    RANLIB="arm-none-eabi-gcc-ranlib",
    SIZETOOL="arm-none-eabi-size",

    LDSCRIPT_PATH = join(FRAMEWORK_DIR, "ldscripts", "MKL25Z4.ld"),

    UPLOADERFLAGS=[
        "-f", "interface/cmsis-dap.cfg",
        "-f", "board/frdm-kl25z.cfg",
        "-c", "init; halt;",
        "-c", "program {$SOURCE} verify; reset; exit 0;"
    ],
)


env.Append(
    ASFLAGS = ["-mcpu=cortex-m0plus", "-mthumb", "-mfloat-abi=soft"],
    CCFLAGS=["-mcpu=cortex-m0plus", "-mthumb", "-mfloat-abi=soft"],
    LINKFLAGS=["-mcpu=cortex-m0plus", "-mthumb", "-mfloat-abi=soft"],
    CPPDEFINES = [],

    CPPPATH = [
        join(FRAMEWORK_DIR, "include"),
        join(FRAMEWORK_DIR, "drivers")
    ],

    LIBS = [
        env.BuildLibrary( join("$BUILD_DIR", "phnx02"), 
        FRAMEWORK_DIR, ["+<startup/*.c>", "+<drivers/*.c>"])
    ]

)