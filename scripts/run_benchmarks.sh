
#!/usr/bin/env bash

set -e

PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"

echo "===> Running benchmarks"
echo "Project root: $PROJECT_ROOT"
echo "Build dir   : $BUILD_DIR"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake ..
mingw32-make

if [ -f "./time_benchmark" ]; then
    echo
    echo "===> time_benchmark"
    ./time_benchmark
else
    echo "time_benchmark not found"
fi

if [ -f "./memory_benchmark" ]; then
    echo
    echo "===> memory_benchmark"
    ./memory_benchmark
else
    echo "memory_benchmark not found"
fi
