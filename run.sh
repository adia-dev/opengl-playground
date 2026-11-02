#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR"

PRESET="${CMAKE_PRESET:-debug}"
BUILD_DIR="${BUILD_DIR:-build}"

cmake --preset "$PRESET"
cmake --build --preset "$PRESET" --target 3d_balls
exec "$PROJECT_DIR/$BUILD_DIR/3d_balls" "$@"
