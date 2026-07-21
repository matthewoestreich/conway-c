#include "grid.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

void grid_init_cells(Grid* g) {
    for (size_t y = 0; y < g->rows; ++y) {
        for (size_t x = 0; x < g->cols; ++x) {
            size_t index = (y * g->cols) + x;
            Cell c = {.alive = false, .x = x, .y = y};
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

size_t grid_index(const Grid* g, uint32_t x, uint32_t y) {
    return ((size_t)y * g->cols) + x;
}

Cell* grid_cell(Grid* g, uint32_t x, uint32_t y) {
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

void cell_print(const Cell* c) {
    assert(c != NULL);
    printf("{.alive = %d, .x = %u, .y = %u},\n", c->alive, c->x, c->y);
}

void grid_print(const Grid* g) {
    assert(g != NULL);
    printf("{\n  .rows = %u,\n  .cols = %u,\n  .cells = [\n", g->rows, g->cols);
    for (size_t i = 0; i < (g->rows * g->cols); ++i) {
        printf("    ");
        cell_print(&g->cells[i]);
    }
    printf("  ]\n}\n");
}

Grid* grid_clone(const Grid* g) {
    assert(g != NULL);
    Grid* clone = grid_new(g->rows, g->cols);
    if (clone == NULL) {
        return NULL;
    }
    memcpy(clone->cells, g->cells, g->size * sizeof(Cell));
    return clone;
}

uint32_t grid_alive_neighbors_len(Grid* g, const Vector2 position) {
    int pos_x = position.x;
    int pos_y = position.y;

    uint32_t alive_neighbors = 0;

    for (int y = (pos_y - 1); y <= (pos_y + 1); ++y) {
        for (int x = (pos_x - 1); x <= (pos_x + 1); ++x) {
            if (y == pos_y && x == pos_x) {
                continue;
            }

            Cell* cell = grid_cell(g, (uint32_t)x, (uint32_t)y);
            if (cell != NULL && cell->alive) {
                alive_neighbors++;
            }
        }
    }

    return alive_neighbors;
}
