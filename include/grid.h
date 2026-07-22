#ifndef GRID_H
#define GRID_H

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

Cell* grid_cell_from_coords(Grid* g, uint32_t x, uint32_t y);

Cell* grid_cell_from_world_pos(Grid* g, Vector2 world_pos);

void grid_drop(Grid* g);

void cell_print(const Cell* c);

void grid_print(const Grid* g);

Grid* grid_clone(const Grid* g);

uint32_t grid_alive_neighbors_len(Grid* g, const Vector2 position);

#endif  // GRID_HEADER
