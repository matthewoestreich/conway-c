#include "conway.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "grid.h"
#include "raylib.h"

Conway conway_new(float update_interval, Grid* grid) {
    Conway cw = {.update_interval = update_interval,
                 .timer = update_interval,
                 .grid = grid};
    return cw;
}

void conway_clicked(Conway* conway, const Vector2* clicked_pos) {
    Cell* cell = grid_cell(conway->grid, (uint32_t)clicked_pos->x,
                           (uint32_t)clicked_pos->y);

    printf("clicked on cell at pos : x= %d, y= %d\n", cell->x, cell->y);

    if (cell != NULL) {
        cell->alive = !cell->alive;
    }
}

void conway_update(Conway* conway, const float delta_time) {
    conway->timer -= delta_time;

    if (conway->timer > 0.0f) {
        return;
    }

    Grid* next_grid = grid_clone(conway->grid);

    for (size_t i = 0; i < next_grid->size; ++i) {
        Cell* cell = &conway->grid->cells[i];
        Vector2 cell_pos = (Vector2){.x = (float)cell->x, .y = (float)cell->y};

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
    conway->timer = conway->update_interval;
}
