#ifndef GRID_IMPL
#define GRID_IMPL

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "raylib.h"

typedef struct {
    bool alive;
    uint32_t x;
    uint32_t y;
} Cell;

typedef struct {
    uint32_t rows;
    uint32_t cols;
    Cell* cells;
    size_t size;
} Grid;

Grid* grid_new(uint32_t rows, uint32_t cols);

size_t grid_index(const Grid* g, uint32_t x, uint32_t y);

Cell* grid_cell(Grid* g, uint32_t x, uint32_t y);

void grid_drop(Grid* g);

void cell_print(const Cell* c);

void grid_print(const Grid* g);

Grid* grid_clone(const Grid* g);

uint32_t grid_alive_neighbors_len(Grid* g, const Vector2 position);

#endif  // GRID_HEADER
