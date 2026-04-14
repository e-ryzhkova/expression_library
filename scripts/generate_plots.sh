
#!/usr/bin/env bash

set -e

PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
OUTPUT_FILE="$PROJECT_ROOT/benchmark_results.txt"

echo "===> Generating benchmark output"
echo "Output file: $OUTPUT_FILE"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake ..
mingw32-make

{
    echo "=== time_benchmark ==="
    if [ -f "./time_benchmark" ]; then
        ./time_benchmark
    else
        echo "time_benchmark not found"
    fi

    echo
    echo "=== memory_benchmark ==="
    if [ -f "./memory_benchmark" ]; then
        ./memory_benchmark
    else
        echo "memory_benchmark not found"
    fi
} > "$OUTPUT_FILE"

echo "Benchmark output saved to: $OUTPUT_FILE"
