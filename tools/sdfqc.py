#!/usr/bin/env python
# 泛海数据Sensor Development Framework
# 初始创意来自ESP-IDF

import sys
import argparse
import os
import subprocess
import multiprocessing
import re
import shutil
import json
import fnmatch

class FatalError(RuntimeError):
    """
    Wrapper class for runtime errors that aren't caused by bugs in idf.py or the build proces.s
    """
    pass

PYTHON = sys.executable

os.environ["PYTHON"] = sys.executable

def _run_tool(tool_name, args, cwd):
    def quote_arg(arg):
        " Quote 'arg' if necessary "
        if " " in arg and not (arg.startswith('"') or arg.startswith("'")):
            return "'" + arg + "'"
        return arg
    display_args = " ".join(quote_arg(arg) for arg in args)
    print('Executing "%s"...' % str(display_args))
    try:
        subprocess.check_call(args, env=os.environ, cwd=cwd)
    except subprocess.CalledProcessError as e:
        raise FatalError("%s failed with exit code %d" % (tool_name, e.returncode))


def do_cloc(args):
    dotool_args = ["cloc"]
    dotool_args += [
        "--include-ext=h,c,cpp,cxx,hpp,s,H,C,CPP,CXX,HPP,S",
        "--by-percent", "cm",
        "--exclude-dir=build,unittest",
        args.project_dir
    ]
    _run_tool("cloc", dotool_args, args.project_dir)

def do_cppcheck(args):
    prj_path = os.path.join(args.project_dir, "build", "compile_commands.json")
    dotool_args = ["cppcheck"]
    dotool_args += [
        "--enable=warning,style,performance,portability",
        "--quiet",
        "--inconclusive",
        "--force",
        "--suppress=variableScope",
        "--platform=unix32",
        "--inline-suppr"
    ]
    if args.addon:
        dotool_args += ["--addon=" + args.addon]
    if os.path.exists(prj_path):
        dotool_args += ["--project=" + prj_path]
    else:
        dotool_args += ["."]
    _run_tool("cppcheck", dotool_args, args.project_dir)


def do_gcovr(args):
    html_path = os.path.join(args.output_dir, "coverage.html")
    dotool_args = ["gcovr"]
    dotool_args += [
        "--source-encoding", "UTF-8",
        "--html-details", html_path,
        "--exclude", "unittest",
        "--print-summary",
        "-j", "4",
        "."
    ]
    _run_tool("gcovr", dotool_args, args.project_dir)
    print("detail data in:", html_path)

def do_format(args):
    files = []
    extensions = "c,cc,cpp,cxx,c++,h,hh,hpp,hxx,h++".split(',')

    for root, _, filenames in os.walk(args.project_dir):
        in_excludes = False
        for ex in args.excludes:
            if root[len(args.project_dir)+1:len(args.project_dir)+len(ex)+1] == ex:
                in_excludes = True
                break
        if in_excludes: continue
        for ext in extensions:
            for filename in fnmatch.filter(filenames, '*.' + ext):
                files.append(os.path.join(root, filename))

    formatted_files = {}
    try:
        for name in files:
            print('Format {}'.format(name))
            formatted = subprocess.check_output("clang-format --style=file -i " + name)
            formatted_files[name] = formatted

    except OSError:
        print("Cannot find clang-format", file=sys.stderr)
        raise

    except subprocess.CalledProcessError as ex:
        print("Running clang-format failed with non-zero status.", file=sys.stderr)
        print("Command    : {}".format(' '.join(ex.cmd)), file=sys.stderr)
        print("Return code: {}".format(str(ex.returncode)), file=sys.stderr)
        raise


ACTIONS = {
    # action name : ( function , description )
    "cloc":                  ( do_cloc, "Count LOC" ),
    "cppcheck":              ( do_cppcheck, "CPP CHECK" ),
    "gcovr":                 ( do_gcovr, "UT COVERAGE" ),
    "format":                ( do_format, "FORMAT" ),
}


def main():
    parser = argparse.ArgumentParser(description='Phoenix SDF QC tool')
    parser.add_argument('-d', '--project-dir', help="Project directory", default=os.getcwd())
    parser.add_argument('-o', '--output-dir', help="output directory for coverage data", default=os.path.join(os.getcwd(),"build"))
    parser.add_argument('-a', '--addon', help="use cppcheck addon", choices=["misra.py", "cert.py"])
    parser.add_argument('-e', '--excludes', help="don't format those subdirs, default=build", nargs='*', default=["build"])
    parser.add_argument('actions', help="Actions, list of [cloc cppcheck gcovr format], default=cloc cppcheck gcovr", nargs='*')

    args = parser.parse_args()
    actions = []
    for action in args.actions:
        if (action in ACTIONS) and (not action in actions):
            actions += [action]
    args.project_dir = os.path.abspath(args.project_dir)
    args.output_dir = os.path.abspath(args.output_dir)
    if not os.path.exists(args.output_dir):
        os.mkdir(args.output_dir)

    if len(actions) == 0:
        actions = ["cloc", "cppcheck", "gcovr"]

    for action in actions:
        (function, description) = ACTIONS[action]
        print()
        print("==================== %s ====================" % (description))
        function(args)


if __name__ == "__main__":
    try:
        main()
    except FatalError as e:
        print(e)
        sys.exit(2)
