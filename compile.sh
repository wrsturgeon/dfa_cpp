#!/bin/bash

# Exit on first error
set -e

export STANDARD="-std=gnu++2b"

# Configure for each OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "Linux unconfigured; TODO"; exit 1
elif [[ "$OSTYPE" == "darwin"* ]]; then
    export INSTALL="brew install -q clang-format llvm opencv"
    export EXTRA_FLAGS="-framework ApplicationServices"
elif [[ "$OSTYPE" == "cygwin" ]]; then
    echo "Get a real computer"; exit 1
elif [[ "$OSTYPE" == "msys" ]]; then
    echo "Get a real computer"; exit 1
elif [[ "$OSTYPE" == "win32" ]]; then
    echo "Get a real computer"; exit 1
else
    echo "Unrecognized OS \"$OSTYPE\""; exit 1
fi

# Configure if not done already
if [ ! -f config/sizes.hpp ]; then
    mkdir -p config
    clang++ $STANDARD $(pkg-config --cflags --libs opencv4) -o bin/configure src/util/configure.cpp $EXTRA_FLAGS
    bin/configure
    rm bin/configure
fi

# Debug if asked to
if [ "$1" = "debug" ]; then
    echo "Compiling with debug info"
    export DEBUG="-g"
    export OPTIM="-Og"
else
    export OPTIM="-Ofast"
fi

# Auto-format
find src -name '*.*' | xargs clang-format -i --style=file

# Update Eigen
git submodule update --remote

# Get OpenCV's raw imread_ (which doesn't necessarily make a new Mat)
# wget https://raw.githubusercontent.com/opencv/opencv/14e4a10312ff12d8085d8dc67d5d834b93c4361f/modules/imgcodecs/src/loadsave.cpp -O src/util/imread.cpp

CLANG_CMD="clang++ $DEBUG $STANDARD -I. -Ieigen -Ieigen/unsupported $(pkg-config --cflags --libs opencv4) -Iinclude src/main.cpp -o bin/main $OPTIM -Wall -Wextra -Werror"

$INSTALL
mkdir -p bin
$CLANG_CMD $EXTRA_FLAGS