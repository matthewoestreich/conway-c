#include "render.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>

#include "conway.h"
#include "grid.h"
#include "options.h"
#include "raylib.h"

// ------------------------------------
// ----------- Viewport ---------------
// ------------------------------------

Viewport viewport_new(int32_t x, int32_t y, int32_t width, int32_t height,
                      uint32_t cols, uint32_t rows) {
    return (Viewport){
        .x = x,
        .y = y,
        .width = width,
        .height = height,
        .cols = cols,
        .rows = rows,
        .cell_size = (Vector2){.x = (float)width / (float)cols,
                               .y = (float)height / (float)rows},
    };
}

Vector2 viewport_grid_to_world(Viewport* vp, Vector2* grid_pos) {
    return (Vector2){.x = (float)vp->x + (grid_pos->x * vp->cell_size.x),
                     .y = (float)vp->y + (grid_pos->y * vp->cell_size.y)};
}

OptionVector2 viewport_world_to_cell(Viewport* vp, Vector2 world_pos) {
    float local_x = world_pos.x - (float)vp->x;
    float local_y = world_pos.y - (float)vp->y;

    if (local_x < 0.0F || local_x >= (float)vp->width || local_y < 0.0F ||
        local_y >= (float)vp->height) {
        return NONE_VECTOR2;
    }

    float x = floorf(local_x / vp->cell_size.x);
    float y = floorf(local_y / vp->cell_size.y);
    return SOME_VECTOR2((Vector2){.x = x, .y = y});
}

bool viewport_is_within_bounds(Viewport* vp, Vector2* pos) {
    float vp_x = (float)vp->x;
    float vp_y = (float)vp->y;
    float vp_width = (float)vp->width;
    float vp_height = (float)vp->height;
    return (pos->x >= vp_x && pos->x <= (vp_x + vp_width) && pos->y >= vp_y &&
            pos->y <= (vp_y + vp_height)) != 0;
}

// ------------------------------------
// ----------- Renderer ---------------
// ------------------------------------

Renderer renderer_new(Viewport vp) { return (Renderer){.viewport = vp}; }

void renderer_draw_cells(Renderer* renderer, Grid* grid) {
    for (size_t i = 0; i < grid->size; ++i) {
        Cell* c = &grid->cells[i];

        Color color = (int)cell_is_curr_gen_alive(c) ? WHITE : BLACK;
        Vector2 cell_pos = grid_get_cell_coords_from_raw_index(grid, i);
        Vector2 pos = viewport_grid_to_world(&renderer->viewport, &cell_pos);

        DrawRectangle((int)pos.x, (int)pos.y,
                      (int)renderer->viewport.cell_size.x,
                      (int)renderer->viewport.cell_size.y, color);
    }
}

void renderer_draw_grid_border(Renderer* renderer) {
    float thickness = 1.0F;
    Color line_color = GRAY;
    float width =
        (float)renderer->viewport.cols * renderer->viewport.cell_size.x;
    float height =
        (float)renderer->viewport.rows * renderer->viewport.cell_size.y;

    // Draw the overall bounding rectangle using the viewport origin
    DrawRectangleLinesEx(
        (Rectangle){
            .x = (float)renderer->viewport.x,
            .y = (float)renderer->viewport.y,
            .width = width,
            .height = height,
        },
        thickness, line_color);

    // Draw vertical lines
    for (uint32_t col = 0; col <= renderer->viewport.cols; ++col) {
        float x_pos = (float)renderer->viewport.x +
                      ((float)col * renderer->viewport.cell_size.x);
        Vector2 start = {.x = x_pos, .y = (float)renderer->viewport.y};
        Vector2 end = {.x = x_pos, .y = (float)renderer->viewport.y + height};
        DrawLineEx(start, end, thickness, line_color);
    }

    // Draw horizontal lines
    for (uint32_t row = 0; row <= renderer->viewport.rows; ++row) {
        float y_pos = (float)renderer->viewport.y +
                      ((float)row * renderer->viewport.cell_size.y);
        Vector2 start = {.x = (float)renderer->viewport.x, .y = y_pos};
        Vector2 end = {.x = (float)renderer->viewport.x + width, .y = y_pos};
        DrawLineEx(start, end, thickness, line_color);
    }
}

void renderer_draw(Renderer* renderer, Conway* conway) {
    renderer_draw_cells(renderer, conway->grid);
    renderer_draw_grid_border(renderer);
}
