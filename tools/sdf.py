#!/usr/bin/env python
# 泛海数据Sensor Development Framework
# 初始创意来自ESP-IDF

import sys
import argparse
import io
import os
import os.path
import subprocess
import multiprocessing
import re
import shutil
import json

class FatalError(RuntimeError):
    """
    Wrapper class for runtime errors that aren't caused by bugs in idf.py or the build proces.s
    """
    pass


# Use this Python interpreter for any subprocesses we launch
PYTHON = sys.executable

# note: os.environ changes don't automatically propagate to child processes,
# you have to pass env=os.environ explicitly anywhere that we create a process
os.environ["PYTHON"] = sys.executable

# Make flavors, across the various kinds of Windows environments & POSIX...
if "MSYSTEM" in os.environ:  # MSYS
    MAKE_CMD = "make"
    MAKE_GENERATOR = "MSYS Makefiles"
elif os.name == 'nt':  # other Windows
    MAKE_CMD = "mingw32-make"
    MAKE_GENERATOR = "MinGW Makefiles"
else:
    MAKE_CMD = "make"
    MAKE_GENERATOR = "Unix Makefiles"

GENERATORS = \
    [
        # ('generator name', 'build command line', 'version command line', 'verbose flag')
        ("Ninja", ["ninja"], ["ninja", "--version"], "-v"),
        (MAKE_GENERATOR, [MAKE_CMD, "-j", str(multiprocessing.cpu_count() + 2)], ["make", "--version"], "VERBOSE=1"),
    ]
GENERATOR_CMDS = dict((a[0], a[1]) for a in GENERATORS)
GENERATOR_VERBOSE = dict((a[0], a[3]) for a in GENERATORS)


def _run_tool(tool_name, args, cwd):
    def quote_arg(arg):
        " Quote 'arg' if necessary "
        if " " in arg and not (arg.startswith('"') or arg.startswith("'")):
            return "'" + arg + "'"
        return arg
    print(args)
    display_args = " ".join(quote_arg(arg) for arg in args)
    print("Running %s in directory %s" % (tool_name, quote_arg(cwd)))
    print('Executing "%s"...' % str(display_args))
    try:
        # Note: we explicitly pass in os.environ here, as we may have set PHNX_SDF there during startup
        subprocess.check_call(args, env=os.environ, cwd=cwd)
    except subprocess.CalledProcessError as e:
        raise FatalError("%s failed with exit code %d" % (tool_name, e.returncode))


def check_environment():
    """
    Verify the environment contains the top-level tools we need to operate

    (cmake will check a lot of other things)
    """
    if not executable_exists(["cmake", "--version"]):
        raise FatalError("'cmake' must be available on the PATH to use idf.py")
    # find the directory idf.py is in, then the parent directory of this, and assume this is PHNX_SDF
    detected_idf_path = os.path.realpath(os.path.join(os.path.dirname(__file__), ".."))
    if "PHNX_SDF" in os.environ:
        set_idf_path = os.path.realpath(os.environ["PHNX_SDF"])
        if set_idf_path != detected_idf_path:
            print("WARNING: PHNX_SDF environment variable is set to %s but idf.py path indicates IDF directory %s. "
                  "Using the environment variable directory, but results may be unexpected..."
                  % (set_idf_path, detected_idf_path))
    else:
        print("Setting PHNX_SDF environment variable: %s" % detected_idf_path)
        os.environ["PHNX_SDF"] = detected_idf_path

    # check Python dependencies
    print("Checking Python dependencies...")
    try:
        subprocess.check_call([os.environ["PYTHON"],
                               os.path.join(os.environ["PHNX_SDF"], "tools", "check_python_dependencies.py")],
                              env=os.environ)
    except subprocess.CalledProcessError:
        raise SystemExit(1)


def executable_exists(args):
    try:
        subprocess.check_output(args)
        return True
    except Exception:
        return False


def detect_cmake_generator():
    """
    Find the default cmake generator, if none was specified. Raises an exception if no valid generator is found.
    """
    for (generator, _, version_check, _) in GENERATORS:
        if executable_exists(version_check):
            return generator
    raise FatalError("To use idf.py, either the 'ninja' or 'GNU make' build tool must be available in the PATH")


def _ensure_build_directory(args, always_run_cmake=False):
    """Check the build directory exists and that cmake has been run there.

    If this isn't the case, create the build directory (if necessary) and
    do an initial cmake run to configure it.

    This function will also check args.generator parameter. If the parameter is incompatible with
    the build directory, an error is raised. If the parameter is None, this function will set it to
    an auto-detected default generator or to the value already configured in the build directory.
    """
    project_dir = args.project_dir
    # Verify the project directory
    if not os.path.isdir(project_dir):
        if not os.path.exists(project_dir):
            raise FatalError("Project directory %s does not exist")
        else:
            raise FatalError("%s must be a project directory")
    if not os.path.exists(os.path.join(project_dir, "CMakeLists.txt")):
        raise FatalError("CMakeLists.txt not found in project directory %s" % project_dir)

    # Verify/create the build directory
    build_dir = args.build_dir
    if not os.path.isdir(build_dir):
        os.makedirs(build_dir)
    cache_path = os.path.join(build_dir, "CMakeCache.txt")
    if not os.path.exists(cache_path) or always_run_cmake:
        if args.generator is None:
            args.generator = detect_cmake_generator()
        try:
            cmake_args = ["cmake", "-G", args.generator]
            # if not args.no_warnings:
            #     cmake_args += ["--warn-uninitialized"]
            if args.no_ccache:
                cmake_args += ["-DCCACHE_DISABLE=1"]
            if args.define_cache_entry:
                cmake_args += ["-D" + d for d in args.define_cache_entry]
            cmake_args += [project_dir]

            _run_tool("cmake", cmake_args, cwd=args.build_dir)
        except Exception:
            # don't allow partially valid CMakeCache.txt files,
            # to keep the "should I run cmake?" logic simple
            if os.path.exists(cache_path):
                os.remove(cache_path)
            raise

    # Learn some things from the CMakeCache.txt file in the build directory
    cache = parse_cmakecache(cache_path)
    try:
        generator = cache["CMAKE_GENERATOR"]
    except KeyError:
        generator = detect_cmake_generator()
    if args.generator is None:
        args.generator = generator  # reuse the previously configured generator, if none was given
    if generator != args.generator:
        raise FatalError("Build is configured for generator '%s' not '%s'. Run 'idf.py fullclean' to start again."
                         % (generator, args.generator))

    try:
        home_dir = cache["CMAKE_HOME_DIRECTORY"]
        if os.path.normcase(os.path.realpath(home_dir)) != os.path.normcase(os.path.realpath(project_dir)):
            raise FatalError("Build directory '%s' configured for project '%s' not '%s'. Run 'idf.py fullclean' to start again."
                             % (build_dir, os.path.realpath(home_dir), os.path.realpath(project_dir)))
    except KeyError:
        pass  # if cmake failed part way, CMAKE_HOME_DIRECTORY may not be set yet


def parse_cmakecache(path):
    """
    Parse the CMakeCache file at 'path'.

    Returns a dict of name:value.

    CMakeCache entries also each have a "type", but this is currently ignored.
    """
    result = {}
    with open(path) as f:
        for line in f:
            # cmake cache lines look like: CMAKE_CXX_FLAGS_DEBUG:STRING=-g
            # groups are name, type, value
            m = re.match(r"^([^#/:=]+):([^:=]+)=(.+)\n$", line)
            if m:
                result[m.group(1)] = m.group(3)
    return result


def build_target(target_name, args):
    """
    Execute the target build system to build target 'target_name'

    Calls _ensure_build_directory() which will run cmake to generate a build
    directory (with the specified generator) as needed.
    """
    _ensure_build_directory(args)
    generator_cmd = GENERATOR_CMDS[args.generator]

    if not args.no_ccache:
        # Setting CCACHE_BASEDIR & CCACHE_NO_HASHDIR ensures that project paths aren't stored in the ccache entries
        # (this means ccache hits can be shared between different projects. It may mean that some debug information
        # will point to files in another project, if these files are perfect duplicates of each other.)
        #
        # It would be nicer to set these from cmake, but there's no cross-platform way to set build-time environment
        # os.environ["CCACHE_BASEDIR"] = args.build_dir
        # os.environ["CCACHE_NO_HASHDIR"] = "1"
        pass
    if args.verbose:
        generator_cmd += [GENERATOR_VERBOSE[args.generator]]

    _run_tool(generator_cmd[0], generator_cmd + [target_name], args.build_dir)

def flash(action, args):
    desc_path = os.path.join(args.build_dir, "project_description.json")
    if not os.path.exists(desc_path):
        _ensure_build_directory(args)
    with open(desc_path, "r") as f:
        project_desc = json.load(f)

    dotool_path = os.path.join(os.environ["PHNX_SDF"], "components",project_desc["target"], "do.py")
    dotool_args = [PYTHON, dotool_path]
    dotool_args += ["flash", project_desc["app_bin"]]
    _run_tool("do.py", dotool_args, args.build_dir)

def gdbserver(action, args):
    desc_path = os.path.join(args.build_dir, "project_description.json")
    if not os.path.exists(desc_path):
        _ensure_build_directory(args)
    with open(desc_path, "r") as f:
        project_desc = json.load(f)

    dotool_path = os.path.join(os.environ["PHNX_SDF"], "components",project_desc["target"], "do.py")
    dotool_args = [PYTHON, dotool_path]
    dotool_args += ["gdbserver", project_desc["app_elf"]]
    _run_tool("do.py", dotool_args, args.build_dir)

def reset_target(action, args):
    desc_path = os.path.join(args.build_dir, "project_description.json")
    if not os.path.exists(desc_path):
        _ensure_build_directory(args)
    with open(desc_path, "r") as f:
        project_desc = json.load(f)

    dotool_path = os.path.join(os.environ["PHNX_SDF"], "components",project_desc["target"], "do.py")
    dotool_args = [PYTHON, dotool_path]
    dotool_args += ["softreset"]
    _run_tool("do.py", dotool_args, args.build_dir)

def monitor(action, args):
    desc_path = os.path.join(args.build_dir, "project_description.json")
    if not os.path.exists(desc_path):
        _ensure_build_directory(args)
    with open(desc_path, "r") as f:
        project_desc = json.load(f)

    if args.port is None:
        args.port = get_default_serial_port()

    dotool_path = os.path.join(os.environ["PHNX_SDF"], "components",project_desc["target"], "do.py")
    dotool_args = [PYTHON, dotool_path]
    dotool_args += ["monitor", args.port]

    if not args.baud is None:
        dotool_args += [args.port]

    _run_tool("do.py", dotool_args, args.build_dir)


def clean(action, args):
    if not os.path.isdir(args.build_dir):
        print("Build directory '%s' not found. Nothing to clean." % args.build_dir)
        return
    build_target("clean", args)


def reconfigure(action, args):
    _ensure_build_directory(args, True)


def _delete_windows_symlinks(directory):
    """
    It deletes symlinks recursively on Windows. It is useful for Python 2 which doesn't detect symlinks on Windows.
    """
    deleted_paths = []
    if os.name == 'nt':
        import ctypes
        for root, dirnames, filenames in os.walk(directory):
            for d in dirnames:
                full_path = os.path.join(root, d)
                try:
                    full_path = full_path.decode('utf-8')
                except Exception:
                    pass
                if ctypes.windll.kernel32.GetFileAttributesW(full_path) & 0x0400:
                    os.rmdir(full_path)
                    deleted_paths.append(full_path)
    return deleted_paths


def fullclean(action, args):
    build_dir = args.build_dir
    if not os.path.isdir(build_dir):
        print("Build directory '%s' not found. Nothing to clean." % build_dir)
        return
    if len(os.listdir(build_dir)) == 0:
        print("Build directory '%s' is empty. Nothing to clean." % build_dir)
        return

    if not os.path.exists(os.path.join(build_dir, "CMakeCache.txt")):
        raise FatalError("Directory '%s' doesn't seem to be a CMake build directory. Refusing to automatically "
                         "delete files in this directory. Delete the directory manually to 'clean' it." % build_dir)
    red_flags = ["CMakeLists.txt", ".git", ".svn"]
    for red in red_flags:
        red = os.path.join(build_dir, red)
        if os.path.exists(red):
            raise FatalError("Refusing to automatically delete files in directory containing '%s'. Delete files manually if you're sure." % red)
    # OK, delete everything in the build directory...
    # Note: Python 2.7 doesn't detect symlinks on Windows (it is supported form 3.2). Tools promising to not
    # follow symlinks will actually follow them. Deleting the build directory with symlinks deletes also items
    # outside of this directory.
    deleted_symlinks = _delete_windows_symlinks(build_dir)
    if args.verbose and len(deleted_symlinks) > 1:
        print('The following symlinks were identified and removed:\n%s' % "\n".join(deleted_symlinks))
    for f in os.listdir(build_dir):  # TODO: once we are Python 3 only, this can be os.scandir()
        f = os.path.join(build_dir, f)
        if args.verbose:
            print('Removing: %s' % f)
        if os.path.isdir(f):
            shutil.rmtree(f)
        else:
            os.remove(f)

def new_project(action, args):
    def select(prompt, choices):
        ret = ""
        while ret == "":
            print(prompt)
            for i in range(len(choices)):
                print("(%d) %s" % (i+1, choices[i]))
            try:
                choice = int(input("Input your choice : "))
                ret = choices[choice-1]
            except:
                pass
        return ret

    def copyInFile(srcDir,dstDir,filename, target):
        o = open(os.path.join(srcDir,filename + ".json.in"),'r',encoding='utf-8')
        d = open(os.path.join(dstDir,filename + ".json"),'w',encoding='utf-8')
        sdfDir = os.environ["PHNX_SDF"]
        sdfDir = sdfDir.replace("\\","/")
        # 循环读取旧文件
        for line in o:
            line = line.replace("@PHNX_SDF@", sdfDir)
            line = line.replace("@TARGET@",target)
            # line = line.replace("@CMAKE_C_COMPILER@",target)
            # line = line.replace("@CMAKE_DEBUGGER@",target)
            # line = line.replace("@CMAKE_BINARY_DIR@",target)
            # line = line.replace("@@PROJECT_EXECUTABLE@",target)
            d.write(line)
        o.close()
        d.close()

    print()
    # 选择目标环境和项目模板
    project_name = input("Input a name for the new project: ")
    target = select("Target for the project : ", ["fdm32vs10x","gd32vf10x"])
    projectDir = os.path.join(".", project_name)
    targetDir = os.path.join(os.environ["PHNX_SDF"], "components", target)
    examplesDir = os.path.join(os.environ["PHNX_SDF"], "examples", target)

    if os.path.isdir(projectDir):
        answer = input("Subdirectory [%s] already exist, make it a SDF project?[Y/N]" % project_name)
        if answer != "Y" and answer != "y": return
    else:
        examples = []
        for root, dirs, files in os.walk(examplesDir, topdown=False):
            if ('main.c' in files) or ('app.c' in files):
                root[len(examplesDir):]
                examples.append(root[len(examplesDir)+1:])

        if len(examples) == 0:
            print("Examples directory '%s' is empty." % examplesDir)
            return
        template = select("Template for the project : ", examples)

        # 创建工程
        templateDir = os.path.join(examplesDir, template)
        shutil.copytree(templateDir, projectDir)

    try:
        # 在新工程中创建.vscode目录
        vscodeExtInDir = os.path.join(targetDir, ".project", "vscode")
        vscodeExtDir = os.path.join(projectDir, ".vscode")
        os.mkdir(vscodeExtDir)
        copyInFile(vscodeExtInDir, vscodeExtDir, "tasks", target)
        copyInFile(vscodeExtInDir, vscodeExtDir, "c_cpp_properties", target)
        copyInFile(vscodeExtInDir, vscodeExtDir, "extensions", target)
        copyInFile(vscodeExtInDir, vscodeExtDir, "launch", target)

        # 拷贝工程文件
        projectInDir = os.path.join(targetDir, ".project")
        if not os.path.exists(os.path.join(projectDir,".clang-format")):
            shutil.copy(os.path.join(projectInDir,".clang-format"), projectDir)
        if not os.path.exists(os.path.join(projectDir,".editorconfig")):
            shutil.copy(os.path.join(projectInDir,".editorconfig"), projectDir)
        if not os.path.exists(os.path.join(projectDir,".gitignore")):
            shutil.copy(os.path.join(projectInDir,".gitignore"), projectDir)
        if not os.path.exists(os.path.join(projectDir,"CMakeLists.txt")):
            shutil.copy(os.path.join(projectInDir,"CMakeLists.txt"), projectDir)
        if not os.path.exists(os.path.join(projectDir,"README.md")):
            shutil.copy(os.path.join(projectInDir,"README.md"), projectDir)
        if not os.path.exists(os.path.join(projectDir,"openocd.cfg")):
            shutil.copy(os.path.join(targetDir,"openocd.cfg"), projectDir)
    except:
        pass

    # 完成
    print("Project [%s] generated. Use VSCode to open [%s] subdirectory." % (project_name,project_name))
    print("Or, use \"code %s\" command if VSCode correcttly set in your PATH." % (project_name))

def print_closing_message(args):
    actions = str(args.actions)
    if "build" in actions and (not "flash" in actions):
        print("\nBuild complete. To flash, run this command with 'flash' action.")
    else :
        print("Done")

ACTIONS = {
    # action name : ( function (or alias), dependencies, order-only dependencies )
    "all":                   (build_target, [], ["reconfigure", "menuconfig", "clean", "fullclean"]),
    "build":                 ("all",        [], []),  # build is same as 'all' target
    "clean":                 (clean,        [], ["fullclean"]),
    "fullclean":             (fullclean,    [], []),
    "reconfigure":           (reconfigure,  [], ["menuconfig"]),
    "menuconfig":            (build_target, [], []),
    "flash":                 (flash,        ["all"], []),
    "debug":                 (gdbserver,    ["all"], []),
    "monitor":               (monitor,      [], ["flash"]),
    "new":                   (new_project,  [], []),
    "softreset":             (reset_target,  [], []),
}


def get_commandline_options():
    """ Return all the command line options up to but not including the action """
    result = []
    for a in sys.argv:
        if a in ACTIONS.keys():
            break
        else:
            result.append(a)
    return result


def get_default_serial_port():
    """ Return a default serial port. esptool can do this (smarter), but it can create
    inconsistencies where esptool.py uses one port and idf_monitor uses another.

    Same logic as esptool.py search order, reverse sort by name and choose the first port.
    """
    # Import is done here in order to move it after the check_environment() ensured that pyserial has been installed
    import serial.tools.list_ports

    ports = list(reversed(sorted(
        p.device for p in serial.tools.list_ports.comports())))
    try:
        print("Choosing default port %s (use '-p PORT' option to set a specific serial port)" % ports[0])
        return ports[0]
    except IndexError:
        raise RuntimeError("No serial ports found. Connect a device, or use '-p PORT' option to set a specific port.")


def main():
    parser = argparse.ArgumentParser(description='Phoenix IDF build management tool')
    parser.add_argument('-p', '--port', help="Serial port",
                        default=os.environ.get('ESPPORT', None))
    parser.add_argument('-b', '--baud', help="Baud rate")
    # parser.add_argument('-b', '--baud', help="Baud rate",
    #                     default=os.environ.get('ESPBAUD', "115200"))
    parser.add_argument('-C', '--project-dir', help="Project directory", default=os.getcwd())
    parser.add_argument('-B', '--build-dir', help="Build directory", default=None)
    parser.add_argument('-G', '--generator', help="Cmake generator", choices=GENERATOR_CMDS.keys())
    parser.add_argument('-n', '--no-warnings', help="Disable Cmake warnings", action="store_true")
    parser.add_argument('-v', '--verbose', help="Verbose build output", action="store_true")
    parser.add_argument('-D', '--define-cache-entry', help="Create a cmake cache entry", nargs='+')
    parser.add_argument('--no-ccache', help="Disable ccache. Otherwise, if ccache is available on the PATH then it will be used for faster builds.",
                        action="store_true")
    parser.add_argument('actions', help="Actions (build targets or other operations)", nargs='+',
                        choices=ACTIONS.keys())

    args = parser.parse_args()

    check_environment()

    # Advanced parameter checks
    if args.build_dir is not None and os.path.realpath(args.project_dir) == os.path.realpath(args.build_dir):
        raise FatalError("Setting the build directory to the project directory is not supported. Suggest dropping "
                         "--build-dir option, the default is a 'build' subdirectory inside the project directory.")
    if args.build_dir is None:
        args.build_dir = os.path.join(args.project_dir, "build")
    args.build_dir = os.path.realpath(args.build_dir)

    completed_actions = set()

    def execute_action(action, remaining_actions):
        (function, dependencies, order_dependencies) = ACTIONS[action]
        # very simple dependency management, build a set of completed actions and make sure
        # all dependencies are in it
        for dep in dependencies:
            if dep not in completed_actions:
                execute_action(dep, remaining_actions)
        for dep in order_dependencies:
            if dep in remaining_actions and dep not in completed_actions:
                execute_action(dep, remaining_actions)

        if action in completed_actions:
            pass  # we've already done this, don't do it twice...
        elif function in ACTIONS:  # alias of another action
            execute_action(function, remaining_actions)
        else:
            function(action, args)

        completed_actions.add(action)

    actions = list(args.actions)

    while len(actions) > 0:
        execute_action(actions[0], actions[1:])
        actions.pop(0)

    print_closing_message(args)


if __name__ == "__main__":
    try:
        # On MSYS2 we need to run idf.py with "winpty" in order to be able to cancel the subprocesses properly on
        # keyboard interrupt (CTRL+C).
        # Using an own global variable for indicating that we are running with "winpty" seems to be the most suitable
        # option as os.environment['_'] contains "winpty" only when it is run manually from console.
        WINPTY_VAR = 'WINPTY'
        WINPTY_EXE = 'winpty'
        if ('MSYSTEM' in os.environ) and (not os.environ['_'].endswith(WINPTY_EXE) and WINPTY_VAR not in os.environ):
            os.environ[WINPTY_VAR] = '1'    # the value is of no interest to us
            # idf.py calls itself with "winpty" and WINPTY global variable set
            ret = subprocess.call([WINPTY_EXE, sys.executable] + sys.argv, env=os.environ)
            if ret:
                raise SystemExit(ret)
        else:
            main()
    except FatalError as e:
        print(e)
        sys.exit(2)
