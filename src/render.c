#include "render.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "conway.h"
#include "grid.h"
#include "options.h"
#include "raylib.h"

// ------------------------------------------------------------------------------------
// -------------- Viewport ------------
// ------------------------------------------------------------------------------------

Viewport viewport_new(int32_t x, int32_t y, int32_t width, int32_t height,
                      uint32_t cols, uint32_t rows) {
    Viewport vp;
    vp.x = x;
    vp.y = y;
    vp.width = width;
    vp.height = height;
    vp.cols = cols;
    vp.rows = rows;

    vp.cell_size.x = width / (float)cols;
    vp.cell_size.y = height / (float)rows;

    return vp;
}

Vector2 viewport_grid_to_world(Viewport* vp, Vector2* grid_pos) {
    Vector2 world_pos;
    world_pos.x = vp->x + (grid_pos->x * vp->cell_size.x);
    world_pos.y = vp->y + (grid_pos->y * vp->cell_size.y);
    return world_pos;
}

OptionVector2 viewport_world_to_cell(Viewport* vp, Vector2 world_pos) {
    float local_x = world_pos.x - vp->x;
    float local_y = world_pos.y - vp->y;

    if (local_x < 0.0f || local_x >= vp->width || local_y < 0.0f ||
        local_y >= vp->height) {
        return NONE_VECTOR2;
    }

    float x = floorf(local_x / vp->cell_size.x);
    float y = floorf(local_y / vp->cell_size.y);
    return SOME_VECTOR2((Vector2){.x = x, .y = y});
}

bool viewport_is_within_bounds(Viewport* vp, Vector2* pos) {
    return pos->x >= vp->x && pos->x <= (vp->x + vp->width) &&
           pos->y >= vp->y && pos->y <= (vp->y + vp->height);
}

// ------------------------------------------------------------------------------------
// -------------- Renderer ------------
// ------------------------------------------------------------------------------------

Renderer renderer_new(Viewport vp) { return (Renderer){.viewport = vp}; }

void renderer_draw_cells(Renderer* renderer, Grid* grid) {
    for (size_t i = 0; i < grid->size; ++i) {
        Cell* c = &grid->cells[i];

        Color color = c->alive ? WHITE : BLACK;
        Vector2 c_pos = (Vector2){.x = c->x, .y = c->y};
        Vector2 pos = viewport_grid_to_world(&renderer->viewport, &c_pos);

        DrawRectangle((int)pos.x, (int)pos.y,
                      (int)renderer->viewport.cell_size.x,
                      (int)renderer->viewport.cell_size.y, color);
    }
}

void renderer_draw_grid_border(Renderer* renderer, Grid* grid) {
    float thickness = 1.0f;
    Color line_color = GRAY;
    float width =
        (float)renderer->viewport.cols * renderer->viewport.cell_size.x;
    float height =
        (float)renderer->viewport.rows * renderer->viewport.cell_size.y;

    // Draw the overall bounding rectangle using the viewport origin
    DrawRectangleLinesEx(
        (Rectangle){
            .x = renderer->viewport.x,
            .y = renderer->viewport.y,
            .width = width,
            .height = height,
        },
        thickness, line_color);

    // Draw vertical lines
    for (uint32_t col = 0; col <= renderer->viewport.cols; ++col) {
        float x_pos = renderer->viewport.x +
                      ((float)col * renderer->viewport.cell_size.x);
        Vector2 start = {.x = x_pos, .y = renderer->viewport.y};
        Vector2 end = {.x = x_pos, .y = renderer->viewport.y + height};
        DrawLineEx(start, end, thickness, line_color);
    }

    // Draw horizontal lines
    for (uint32_t row = 0; row <= renderer->viewport.rows; ++row) {
        float y_pos = renderer->viewport.y +
                      ((float)row * renderer->viewport.cell_size.y);
        Vector2 start = {.x = renderer->viewport.x, .y = y_pos};
        Vector2 end = {.x = renderer->viewport.x + width, .y = y_pos};
        DrawLineEx(start, end, thickness, line_color);
    }
}

void renderer_draw(Renderer* renderer, Conway* conway) {
    Grid* grid = conway->grid;
    renderer_draw_cells(renderer, grid);
    renderer_draw_grid_border(renderer, grid);
}
