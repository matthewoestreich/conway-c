#!/bin/bash

(
  cd "$(dirname "$0")"

  BUILD_DIR="./build/"
  mkdir -p "$BUILD_DIR"

  clang \
    ./src/*.c \
    ./vendor/raylib_6_0/lib/libraylib.a \
    -I./include \
    -I./vendor/raylib_6_0/include \
    -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo \
    -o "$BUILD_DIR/main" \
    -Wall \
    -Wextra \
    --verbose
)
