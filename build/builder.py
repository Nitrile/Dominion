#!/bin/python3

import sys, os, subprocess
import argparse

# Parse arguments using argparse

parser = argparse.ArgumentParser()
group_invoke = parser.add_mutually_exclusive_group()

# Invocation types
group_invoke.add_argument("-t", "--test",
                          action="store_true",
                          help="Execute test")
group_invoke.add_argument("-b", "--build",
                          action="store_true",
                          help="Build project")
group_invoke.add_argument("-p", "--proj",
                          action="store_true",
                          help="Configure project")
group_invoke.add_argument("-c", "--clean",
                          action="store_true",
                          help="Clean (If a level is not specified then it cleans all of them)")

# Options
LEVELS = ["Debug", "Exp", "Release"]
parser.add_argument("-l", "--level",
                    type=str, choices = LEVELS, default = "Debug",
                    help = "Build Type")
parser.add_argument("-g", "--generator",
                    type=str, default = "",
                    help = "Project Generator")
args = parser.parse_args()

# Process arguments

CMAKE_BUILD_TYPE = args.level
BASE_DIR = os.environ['DEV']
BINARY_DIR = BASE_DIR + "/bin_" + args.level.lower()
BINARY_APP_DIR = BINARY_DIR + "/app"
CMAKE_DIR = BINARY_DIR + "/build"

os.environ['BIN'] = BINARY_DIR

if CMAKE_BUILD_TYPE == "Exp":
    CMAKE_BUILD_TYPE = "RelWithDebugInfo"
if args.proj:
    print("Base Directory: " + BASE_DIR)

    try:
        os.makedirs(BINARY_DIR, exist_ok=True)
        os.makedirs(BINARY_APP_DIR, exist_ok=True)
        os.makedirs(BINARY_DIR + "/lib", exist_ok=True)
        os.makedirs(BINARY_DIR + "/deploy", exist_ok=True)
        os.makedirs(BINARY_DIR + "/build", exist_ok=True)

        if os.path.exists(BASE_DIR + "/bin"):
            os.remove(BASE_DIR + "/bin") # Should be a symlink
        os.symlink(BINARY_DIR, BASE_DIR + "/bin")
    except OSError as e:
        raise

    assert os.path.exists(BINARY_DIR)
    assert os.path.exists(BINARY_APP_DIR)
    assert os.path.exists(BINARY_DIR + "/lib")
    assert os.path.exists(BINARY_DIR + "/deploy")
    assert os.path.exists(BINARY_DIR + "/build")

    os.chdir(BINARY_DIR + "/build")

    CMAKE_COMMAND = ["cmake", "../..",
                     "-DCMAKE_BUILD_TYPE="+CMAKE_BUILD_TYPE
                    ]
    if args.generator != "":
        CMAKE_COMMAND += ["-G", args.generator]
    status = subprocess.call(CMAKE_COMMAND)
    if status != 0:
        print("CMake Configure Failed")
        sys.exit(status)
elif args.build:
    if not os.path.exists(CMAKE_DIR):
        print("CMake build directory does not exist.\n"
              "Please configure project.\n"
             )
        sys.exit(1)
    os.chdir(BASE_DIR)
    print("Building in: " + CMAKE_DIR)
    CMAKE_COMMAND = ["cmake",
                     "--build", CMAKE_DIR,
                     "--config", CMAKE_BUILD_TYPE
                    ]
    status = subprocess.call(CMAKE_COMMAND)
    if status != 0:
        print("CMake Failed")
        sys.exit(status)
elif args.test:
    print("Test not implemented yet")
    sys.exit(1)
elif args.clean:
    if os.path.exists(BASE_DIR + "/bin"):
        os.remove(BASE_DIR + "/bin") # Should be a symlink
    for l in LEVELS:
        path = BASE_DIR + "/bin_" + l.lower()
        if os.path.exists(path):
            subprocess.call(["rm", "-r", path])
else:
    print("No invocation specified. $ builder -h to see available options. Exit.")
    sys.exit(1)

