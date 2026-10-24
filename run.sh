#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR"

cmake --preset debug
cmake --build --preset debug
exec "$PROJECT_DIR/build/3d_balls" "$@"
