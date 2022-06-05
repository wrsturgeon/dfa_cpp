#!/bin/bash

# Exit on first error
set -e
# set -x

export COMPILER=clang++ # zapcc?
export STANDARD=gnu++2b

# Configure for each OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    export OS=linux
    echo "Linux unconfigured; TODO"; exit 1
elif [[ "$OSTYPE" == "darwin"* ]]; then
    export OS=mac
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

# Configure
if [ ! -f "config.hpp" ]; then
    clang++ -o configure src/util/${OS}/configure.cpp $EXTRA_FLAGS
    ./configure
    rm configure
fi

# Debug if asked to
if [ "$1" = "debug" ]; then
    echo "Compiling with debug info"
    export ASAN_SYMBOLIZER_PATH=$(which llvm-symbolizer)
    export DEBUG="-g -fsanitize=address" # -D_FORTIFY_SOURCE=2
fi

# Auto-format
find src -name '*.hpp' -o -name '*.cpp' | xargs clang-format -i --style=file

# Check internet connection
if wget -q --spider http://ismycomputeron.com; then

  # Update Eigen
  git submodule update --remote

  # Pull xoshiro256+
  echo -e "#ifndef UTIL_XOSHIRO_HPP_\n#define UTIL_XOSHIRO_HPP_\n\n//NOTE: THIS IS NOT MINE, just wgetting it each compilation\n//   Source: https://prng.di.unimi.it/xoshiro256plus.c\n\nnamespace xoshiro {\n\n// Clang doesn't like jump()'s different-sign comparison\n#pragma clang diagnostic push\n#pragma clang diagnostic ignored \"-Wsign-compare\"\n" > src/util/xoshiro.hpp
  wget https://prng.di.unimi.it/xoshiro256plus.c -O ->> src/util/xoshiro.hpp
  echo -e "\n#pragma clang diagnostic pop\n\n} // namespace xoshiro\n\n#endif // UTIL_XOSHIRO_HPP_" >> src/util/xoshiro.hpp

else echo "Can't connect to the Internet (or ismycomputeron.com is down lol)"; fi

# If we ever want to put OpenCV back in:
# $(pkg-config --cflags --libs opencv4) generates all clang flags we need
COMPILE_CMD="$COMPILER src/main.cpp -o bin/main$1 -std=$STANDARD $DEBUG -funit-at-a-time -march=native -Ofast -mllvm -polly -mllvm -polly-vectorizer=stripmine -include src/util/${OS}/screenshot.hpp -pedantic -Wall -Wextra -Werror -Wno-c99-extensions" # -Wno-c99-extensions for Eigen

mkdir -p bin
rm -rf out
mkdir out
$COMPILE_CMD $EXTRA_FLAGS

# To debug on Mac, run:
# ./compile.sh debug && leaks -atExit -- bin/main
# Note that Address Sanitizer hugely increases stack usage
#   and can therefore sometimes cause a stack overflow,
#   which it will report as an issue with your program.
#   Not sure how to fix this at the moment.