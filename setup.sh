#!/bin/bash

DEV=`dirname "$BASH_SOURCE"`
export DEV=`realpath $DEV`
export B=$DEV/build
export S=$DEV/src
export SC=$DEV/src/Client
export SS=$DEV/src/Server
export L=$DEV//lib
export T=$DEV/test
export BIN=$DEV/bin

echo "Setting up development environment at $DEV"

alias builder="python3 $DEV/build/builder.py $@"
alias client="$DEV/bin/app/Client"
alias server="$DEV/bin/app/Server"
