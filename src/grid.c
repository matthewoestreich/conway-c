#include "grid.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

Cell cell_new(bool is_alive) { return (Cell)is_alive; }

void cell_set_curr_gen_alive(Cell* cell, bool is_alive) {
    // Clear first bit then XOR
    *cell = (*cell & CELL_MASK_CLEAR_CURR) | is_alive;
}

void cell_set_next_gen_alive(Cell* cell, bool is_alive) {
    *cell = (*cell & CELL_MASK_CLEAR_NEXT) | (is_alive << 1);
}

bool cell_is_curr_gen_alive(const Cell* c) { return *c & CELL_MASK_READ_CURR; }

void cell_advance_gen(Cell* cell) { *cell >>= 1; }

static void grid_init_cells(Grid* g) {
    for (size_t y = 0; y < g->rows; ++y) {
        for (size_t x = 0; x < g->cols; ++x) {
            size_t index = (y * g->cols) + x;
            Cell c = cell_new(false);  // {.alive = false};
            g->cells[index] = c;
        }
    }
}

Grid* grid_new(uint32_t rows, uint32_t cols) {
    Grid* g = malloc(sizeof(Grid));

    if (g == NULL) {
        return NULL;
    }

    size_t size = (rows * cols);

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

static size_t grid_index(const Grid* g, uint32_t x, uint32_t y) {
    return ((size_t)y * g->cols) + x;
}

Vector2 grid_get_cell_pos_from_raw_index(Grid* g, size_t raw_index) {
    size_t width = (size_t)g->cols;
    size_t x = raw_index % width;
    size_t y = raw_index / width;
    return (Vector2){.x = x, .y = y};
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
uint32_t grid_alive_neighbors_len(Grid* g, const size_t raw_cell_index) {
    Vector2 cell_pos = grid_get_cell_pos_from_raw_index(g, raw_cell_index);
    int pos_x = cell_pos.x;
    int pos_y = cell_pos.y;

    uint32_t alive_neighbors = 0;

    for (int y = (pos_y - 1); y <= (pos_y + 1); ++y) {
        for (int x = (pos_x - 1); x <= (pos_x + 1); ++x) {
            if (y == pos_y && x == pos_x) {
                continue;
            }

            Cell* neighbor = grid_cell_from_coords(g, (uint32_t)x, (uint32_t)y);
            if (neighbor != NULL && cell_is_curr_gen_alive(neighbor)) {
                alive_neighbors++;
            }
        }
    }

    return alive_neighbors;
}
