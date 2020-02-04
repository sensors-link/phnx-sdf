import sys
from os.path import join

from SCons.Script import (ARGUMENTS, COMMAND_LINE_TARGETS, AlwaysBuild,
                          Default, DefaultEnvironment)


env = DefaultEnvironment()
platform = env.PioPlatform()
board = env.BoardConfig()

# Allow user to override via pre:script
if env.get("PROGNAME", "program") == "program":
    env.Replace(PROGNAME="firmware")

env.Replace(
    ARFLAGS=["rc"],
    SIZEPRINTCMD='$SIZETOOL -d $SOURCES',
    PROGSUFFIX=".elf",
    UPLOADER="openocd",
    # UPLOADERFLAGS=[] # defined in frameworks/*.py
    UPLOADCMD="$UPLOADER $UPLOADERFLAGS"
)


env.Append(
    ASFLAGS = [
        "-ffunction-sections",
        "-fdata-sections",
        # "-Os",
        # "-ggdb"
    ],

    CCFLAGS=[
        # Default compiler configuration
        "-ffunction-sections",
        "-fdata-sections",
        "-fstrict-volatile-bitfields",
        "-Wno-old-style-declaration",
        # "-ggdb",
        # "-Os",
        # Error & Warning
        "-Wall",
        "-Werror=all",
        "-Wno-error=unused-function",
        "-Wno-error=unused-but-set-variable",
        "-Wno-error=unused-variable",
        "-Wno-error=deprecated-declarations",
        "-Wextra",
        "-Wno-unused-parameter",
        "-Wno-sign-compare",
        
    ],


    CPPDEFINES = [
        # "USE_STDPERIPH_DRIVER",
        # ("HXTAL_VALUE", "%sU" % board.get("build.hxtal_value"))
    ],

    LINKFLAGS=[
        "-nostartfiles",
        "-Wl,--gc-sections",
        "--specs=nano.specs"
    ],

)

env.Append(
    BUILDERS=dict(
        ElfToBin=Builder(
            action=env.VerboseAction(" ".join([
                "$OBJCOPY",
                "-O",
                "binary",
                "$SOURCES",
                "$TARGET"
            ]), "Building $TARGET"),
            suffix=".bin"
        ),
        ElfToHex=Builder(
            action=env.VerboseAction(" ".join([
                "$OBJCOPY",
                "-O",
                "ihex",
                "$SOURCES",
                "$TARGET"
            ]), "Building $TARGET"),
            suffix=".hex"
        )
    )
)

#
# Target: Build executable and linkable firmware
#

target_elf = None
if "nobuild" in COMMAND_LINE_TARGETS:
    target_elf = join("$BUILD_DIR", "${PROGNAME}.elf")
    target_firm = join("$BUILD_DIR", "${PROGNAME}.bin")
    target_hex = join("$BUILD_DIR", "${PROGNAME}.hex")
else:
    target_elf = env.BuildProgram()
    target_firm = env.ElfToBin(join("$BUILD_DIR", "${PROGNAME}"), target_elf)
    target_hex = env.ElfToHex(join("$BUILD_DIR", "${PROGNAME}"), target_elf)

AlwaysBuild(env.Alias("nobuild", target_firm))
target_buildprog = env.Alias("buildprog", target_firm, target_firm)
target_buildhex = env.Alias("buildhex", target_hex, target_hex)

#
# Target: Print binary size
#

target_size = env.Alias(
    "size", target_elf,
    env.VerboseAction("$SIZEPRINTCMD", "Calculating size $SOURCE"))
AlwaysBuild(target_size)

AlwaysBuild(env.Alias("upload", join("$BUILD_DIR", "${PROGNAME}.elf"), 
    [env.VerboseAction("$UPLOADCMD", "Uploading $SOURCE")] ))

#
# Setup default targets
#

Default([target_buildprog, target_buildhex, target_size])