#!/usr/bin/env bash

set -e

PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"

echo "===> Running tests"
echo "Project root: $PROJECT_ROOT"
echo "Build dir   : $BUILD_DIR"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake ..
mingw32-make

ctest --output-on-failure

