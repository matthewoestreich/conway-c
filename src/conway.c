#include "conway.h"

#include <stddef.h>
#include <stdint.h>

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

void conway_reset(Conway* conway) { grid_reset_empty(conway->grid); }

void conway_update(Conway* conway, const float delta_time) {
    conway->timer -= delta_time;

    if (conway->timer > 0.0F) {
        return;
    }

    for (size_t i = 0; i < conway->grid->size; ++i) {
        Cell* cell = &conway->grid->cells[i];
        uint32_t alive_neighbors = grid_alive_neighbors_len(conway->grid, i);
        bool alive = cell_is_curr_gen_alive(cell);
        bool survives =
            (((int)alive && (alive_neighbors == 2 || alive_neighbors == 3)) ||
             (!alive && alive_neighbors == 3)) != 0;
        cell_set_next_gen_alive(cell, survives);
    }

    // Advance eacch cell to the next generation
    for (size_t i = 0; i < conway->grid->size; ++i) {
        cell_advance_gen(&conway->grid->cells[i]);
    }

    // Since we return early above (if 'conway->timer > 0.0f') we can
    // safely assume we need to reset our timer here.
    conway->timer = conway->update_interval;
}
