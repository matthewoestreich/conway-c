#ifndef GRID_H
#define GRID_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "raylib.h"

// Clearing Masks (Used to set a bit while leaving other bits alone)
#define CELL_MASK_CLEAR_CURR 0b11111110  // Current gen
#define CELL_MASK_CLEAR_NEXT 0b11111101  // Next gen
// Reading Masks (Used to isolate a bit)
#define CELL_MASK_READ_CURR 0b00000001  // Current gen
#define CELL_MASK_READ_NEXT 0b00000010  // Next gen

typedef unsigned char Cell;

Cell cell_new(bool is_alive);

void cell_set_curr_gen_alive(Cell* cell, bool is_alive);

void cell_set_next_gen_alive(Cell* cell, bool is_alive);

bool cell_is_curr_gen_alive(const Cell* c);

void cell_advance_gen(Cell* c);

typedef struct {
    uint32_t rows;
    uint32_t cols;
    Cell* cells;
    size_t size;
} Grid;

Grid* grid_new(uint32_t rows, uint32_t cols);

Cell* grid_cell_from_coords(Grid* g, uint32_t x, uint32_t y);

Vector2 grid_get_cell_pos_from_raw_index(Grid* g, size_t raw_index);

void grid_drop(Grid* g);

uint32_t grid_alive_neighbors_len(Grid* g, const size_t raw_cell_index);

#endif  // GRID_H
