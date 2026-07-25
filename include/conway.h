#ifndef CONWAY_H
#define CONWAY_H

#include "grid.h"
#include "raylib.h"

typedef struct {
    float update_interval;
    float timer;
    Grid* grid;
} Conway;

Conway conway_new(float update_interval, Grid* grid);

void conway_reset(Conway* conway);

Cell* conway_clicked(Conway* conway, Vector2 clicked_pos);

void conway_update(Conway* conway, float delta_time);

#endif  // CONWAY_H
