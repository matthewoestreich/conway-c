#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>

#include "conway.h"
#include "options.h"
#include "raylib.h"

// ------------------------------------
// ----------- Viewport ---------------
// ------------------------------------

typedef struct {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    uint32_t cols;
    uint32_t rows;
    Vector2 cell_size;
} Viewport;

Viewport viewport_new(int32_t x, int32_t y, int32_t width, int32_t height,
                      uint32_t cols, uint32_t rows);

Vector2 viewport_grid_to_world(Viewport* vp, Vector2* grid_pos);

OptionVector2 viewport_world_to_cell(Viewport* vp, Vector2 world_pos);

bool viewport_is_within_bounds(Viewport* vp, Vector2* pos);

// ------------------------------------
// ----------- Renderer ---------------
// ------------------------------------

typedef struct {
    Viewport viewport;
} Renderer;

Renderer renderer_new(Viewport vp);

void renderer_draw(Renderer* renderer, Conway* conway);

#endif  // RENDER_H
