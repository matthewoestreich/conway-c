#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "conway.h"
#include "grid.h"
#include "raylib.h"
#include "render.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define VIEWPORT_WIDTH 1000
#define VIEWPORT_HEIGHT 800
#define FPS 60
#define ROWS 50
#define COLS 50
#define UPDATE_INTERVAL_SECS 0.3  // 1.0 == 1 second

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello, Raylib!");
    SetTargetFPS(FPS);

    Viewport vp =
        viewport_new(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, COLS, ROWS);

    Renderer renderer = renderer_new(vp);

    Grid* grid = grid_new(ROWS, COLS);
    Conway conway = conway_new(UPDATE_INTERVAL_SECS, grid);

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();
        conway_update(&conway, delta_time);

        BeginDrawing();
        ClearBackground(BLACK);

        renderer_draw(&renderer, &conway);

        EndDrawing();
    }

    CloseWindow();
    grid_drop(grid);

    return 0;
}
