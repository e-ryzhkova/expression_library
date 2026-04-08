
#!/usr/bin/env bash

set -e

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input.dot> <output.png>"
    exit 1
fi

INPUT_DOT="$1"
OUTPUT_PNG="$2"

if [ ! -f "$INPUT_DOT" ]; then
    echo "Input file not found: $INPUT_DOT"
    exit 1
fi

dot -Tpng "$INPUT_DOT" -o "$OUTPUT_PNG"

echo "Generated: $OUTPUT_PNG"
