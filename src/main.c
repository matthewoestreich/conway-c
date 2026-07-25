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
#define FAST_FORWARD_MULTIPLIER 5
#define STABLE_DELTA_TIME 0.01666667
#define TITLE_BUFFER_SIZE 128

static Cell* get_cell_under_mouse(Viewport* vp, Conway* conway) {
    Vector2 mp = GetMousePosition();
    if (!viewport_is_within_bounds(vp, &mp)) {
        return NULL;
    }

    OptionVector2 maybe_pos = viewport_world_to_cell(vp, mp);
    if (maybe_pos.is_some) {
        return conway_clicked(conway, maybe_pos.value);
    }

    return NULL;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway");
    SetTargetFPS(FPS);

    Viewport viewport =
        viewport_new(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, COLS, ROWS);
    Renderer renderer = renderer_new(viewport);
    Grid* grid = grid_new(ROWS, COLS);
    Conway conway = conway_new(UPDATE_INTERVAL_SECS, grid);

    // App state
    char title_buffer[TITLE_BUFFER_SIZE];
    bool is_paused = true;
    bool is_dragging = false;
    bool is_drawing = false;
    bool is_fast_forward = false;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            conway_reset(&conway);
        } else if (IsKeyPressed(KEY_F)) {
            if (!is_paused) {
                is_fast_forward = ((!is_fast_forward) != 0);
            }
        } else if (IsKeyPressed(KEY_SPACE)) {
            is_paused = ((!is_paused) != 0);
        }

        if (is_paused) {
            sprintf(title_buffer, "Conway | PAUSED");
            if (is_fast_forward) {
                is_fast_forward = false;
            }
        } else if (is_fast_forward) {
            sprintf(title_buffer, "Conway | >> FF x%d >>",
                    FAST_FORWARD_MULTIPLIER);
            for (int i = 0; i < FAST_FORWARD_MULTIPLIER; ++i) {
                conway_update(&conway, STABLE_DELTA_TIME);
            }
        } else {
            sprintf(title_buffer, "Conway");
            conway_update(&conway, GetFrameTime());
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Cell* cell = get_cell_under_mouse(&renderer.viewport, &conway);

            if (cell != NULL) {
                is_dragging = true;
                is_drawing = ((!cell_is_curr_gen_alive(cell)) != 0);
                cell_set_curr_gen_alive(cell, is_drawing);
            }
        } else if ((int)IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
                   (int)is_dragging) {
            Cell* cell = get_cell_under_mouse(&renderer.viewport, &conway);

            if (cell != NULL) {
                cell_set_curr_gen_alive(cell, is_drawing);
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
