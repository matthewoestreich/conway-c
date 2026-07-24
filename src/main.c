#include <stdio.h>

#include "conway.h"
#include "grid.h"
#include "options.h"
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
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway");
    SetTargetFPS(FPS);

    Renderer renderer = renderer_new(
        viewport_new(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, COLS, ROWS));

    Conway conway = conway_new(UPDATE_INTERVAL_SECS, grid_new(ROWS, COLS));

    // App state
    char title_buffer[128];
    bool is_paused = true;
    bool is_dragging = false;
    bool is_drawing = false;
    bool is_fast_forward = false;
    int fast_forward_multiplier = 5;
    float stable_delta_time_fast_forward = 0.01666667;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            conway_reset(&conway);
        } else if (IsKeyPressed(KEY_F)) {
            if (!is_paused) {
                is_fast_forward = !is_fast_forward;
            }
        } else if (IsKeyPressed(KEY_SPACE)) {
            is_paused = !is_paused;
        }

        if (is_paused) {
            sprintf(title_buffer, "Conway | PAUSED");
        } else if (is_fast_forward) {
            sprintf(title_buffer, "Conway | >> FF x%d >>",
                    fast_forward_multiplier);
            for (int i = 0; i < fast_forward_multiplier; ++i) {
                conway_update(&conway, stable_delta_time_fast_forward);
            }
        } else {
            sprintf(title_buffer, "Conway");
            conway_update(&conway, GetFrameTime());
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            OptionVector2 maybe_pos =
                viewport_world_to_cell(&renderer.viewport, GetMousePosition());

            if (maybe_pos.is_some) {
                Cell* cell = conway_clicked(&conway, maybe_pos.value);
                if (cell != NULL) {
                    is_dragging = true;
                    is_drawing = !cell_is_curr_gen_alive(cell);
                    cell_set_curr_gen_alive(cell, is_drawing);
                }
            }
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && is_dragging) {
            OptionVector2 maybe_pos =
                viewport_world_to_cell(&renderer.viewport, GetMousePosition());

            if (maybe_pos.is_some) {
                Cell* cell = conway_clicked(&conway, maybe_pos.value);
                if (cell != NULL) {
                    cell_set_curr_gen_alive(cell, is_drawing);
                }
            }
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            is_dragging = false;
        }

        SetWindowTitle(title_buffer);

        BeginDrawing();
        ClearBackground(BLACK);

        renderer_draw(&renderer, &conway);

        EndDrawing();
    }

    CloseWindow();
    grid_drop(conway.grid);

    return 0;
}
