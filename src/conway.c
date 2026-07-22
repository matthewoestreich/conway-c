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

void conway_update(Conway* conway, const float delta_time) {
    conway->timer -= delta_time;

    if (conway->timer > 0.0f) {
        return;
    }

    for (size_t i = 0; i < conway->grid->size; ++i) {
        Cell* cell = &conway->grid->cells[i];
        // IMPORTANT : Reads from current generation bit.
        uint32_t alive_neighbors = grid_alive_neighbors_len(conway->grid, i);

        if (cell_is_curr_gen_alive(cell)) {
            // If a cell that is alive has 2 or 3 alive neighbors it survives.
            bool survives = alive_neighbors == 2 || alive_neighbors == 3;
            // IMPORTANT : set the next generation bit!!!
            cell_set_next_gen_alive(cell, survives);
        } else {
            // Within this 'else' block we know the cell is dead.
            if (alive_neighbors == 3) {
                // If a dead celll has 3 alive neighbors, it can be born again.
                // IMPORTANT : set the next generation bit!!!
                cell_set_next_gen_alive(cell, true);
            }
        }
    }

    // Advance eacch cell to the next generation (shift second bit into first
    // bit position)
    for (size_t i = 0; i < conway->grid->size; ++i) {
        cell_advance_gen(&conway->grid->cells[i]);
    }

    // Since we return early above (if 'conway->timer > 0.0f') we can
    // safely assume we need to reset our timer here.
    conway->timer = conway->update_interval;
}
