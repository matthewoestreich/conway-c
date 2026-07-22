#include "conway.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "grid.h"
#include "raylib.h"

Conway conway_new(float update_interval, Grid* grid) {
    return (Conway){.update_interval = update_interval,
                    .timer = update_interval,
                    .grid = grid};
}

Cell* conway_clicked(Conway* conway, const Vector2 clicked_pos) {
    uint32_t x = (uint32_t)clicked_pos.x;
    uint32_t y = (uint32_t)clicked_pos.y;
    return grid_cell_from_coords(conway->grid, x, y);
}

void conway_update(Conway* conway, const float delta_time) {
    conway->timer -= delta_time;

    if (conway->timer > 0.0f) {
        return;
    }

    Grid* next_grid = grid_clone(conway->grid);
    assert(next_grid != NULL);

    for (size_t i = 0; i < next_grid->size; ++i) {
        Cell* cell = &next_grid->cells[i];
        Vector2 cell_pos = {.x = (float)cell->x, .y = (float)cell->y};

        uint32_t alive_neighbors =
            grid_alive_neighbors_len(conway->grid, cell_pos);

        if (cell->alive) {
            cell->alive = alive_neighbors == 2 || alive_neighbors == 3;
        } else if (alive_neighbors == 3) {
            // Here we can assume the cell is not alive
            // If a dead celll has 3 alive neighbors, we can mark the cell as
            // alive.
            cell->alive = true;
        }
    }

    conway->grid = next_grid;
    // Since we return early above if 'conway->timer > 0.0f' we can
    // safely assume we need to reset our timer here.
    conway->timer = conway->update_interval;
}
