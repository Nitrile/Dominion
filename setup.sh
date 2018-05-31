#!/bin/bash

#SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
DEV=`dirname "$BASH_SOURCE"`
export DEV=`realpath $DEV`
export B=$DEV/build
export C=$DEV/src
export T=$DEV/test

echo "Setting up development environment at $DEV"

alias builder="python3 $DEV/build/builder.py $@"
alias client="$DEV/bin/app/Client"
alias server="$DEV/bin/app/Server"
