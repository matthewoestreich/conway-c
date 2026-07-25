#include "grid.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

Cell cell_new(bool is_alive) { return (Cell)is_alive; }

void cell_set_curr_gen_alive(Cell* cell, bool is_alive) {
    assert(cell != NULL);
    *cell = (*cell & CELL_MASK_CLEAR_CURR) | (int)is_alive;
}

void cell_set_next_gen_alive(Cell* cell, bool is_alive) {
    assert(cell != NULL);
    *cell = (*cell & CELL_MASK_CLEAR_NEXT) | ((int)is_alive << 1);
}

bool cell_is_curr_gen_alive(const Cell* cell) {
    assert(cell != NULL);
    return (*cell & CELL_MASK_READ_CURR) != 0;
}

void cell_advance_gen(Cell* cell) {
    assert(cell != NULL);
    bool next_alive = (*cell & CELL_MASK_READ_NEXT) != 0;
    cell_set_curr_gen_alive(cell, next_alive);
}

static void grid_init_cells(Grid* g) {
    assert(g != NULL && g->cells != NULL);

    for (size_t y = 0; y < g->rows; ++y) {
        for (size_t x = 0; x < g->cols; ++x) {
            size_t index = (y * g->cols) + x;
            Cell c = cell_new(false);
            g->cells[index] = c;
        }
    }
}

Grid* grid_new(uint32_t rows, uint32_t cols) {
    Grid* g = malloc(sizeof(Grid));

    if (g == NULL) {
        return NULL;
    }

    size_t size = ((size_t)(rows * cols));

    g->rows = rows;
    g->cols = cols;
    g->size = size;
    g->cells = malloc(sizeof(Cell) * size);

    if (g->cells == NULL) {
        free(g);
        return NULL;
    }

    grid_init_cells(g);

    return g;
}

void grid_reset_empty(Grid* grid) {
    if (grid == NULL || grid->cells == NULL) {
        return;
    }
    memset(grid->cells, 0, sizeof(Cell) * grid->size);
}

static size_t grid_index(const Grid* g, uint32_t x, uint32_t y) {
    return ((size_t)y * g->cols) + x;
}

Vector2 grid_get_cell_coords_from_raw_index(Grid* g, size_t raw_index) {
    assert(g != NULL);
    size_t width = (size_t)g->cols;
    size_t x = raw_index % width;
    size_t y = raw_index / width;
    return (Vector2){.x = (float)x, .y = (float)y};
}

Cell* grid_cell_from_coords(Grid* g, uint32_t x, uint32_t y) {
    if (g == NULL || x >= g->cols || y >= g->rows) {
        return NULL;
    }
    size_t idx = grid_index(g, x, y);
    return &g->cells[idx];
}

void grid_drop(Grid* g) {
    assert(g != NULL);
    free(g->cells);
    free(g);
}

/// Gets the CURRENT GEN alive neighbors
uint32_t grid_alive_neighbors_len(Grid* g, size_t raw_cell_index) {
    assert(g != NULL);

    Vector2 cell_pos = grid_get_cell_coords_from_raw_index(g, raw_cell_index);
    int pos_x = (int)cell_pos.x;
    int pos_y = (int)cell_pos.y;

    uint32_t alive_neighbors = 0;

    for (int y = (pos_y - 1); y <= (pos_y + 1); ++y) {
        for (int x = (pos_x - 1); x <= (pos_x + 1); ++x) {
            if (y == pos_y && x == pos_x) {
                continue;
            }

            Cell* neighbor = grid_cell_from_coords(g, (uint32_t)x, (uint32_t)y);
            if (neighbor != NULL && (int)cell_is_curr_gen_alive(neighbor)) {
                alive_neighbors++;
            }
        }
    }

    return alive_neighbors;
}
