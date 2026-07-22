#ifndef CONWAY_H
#define CONWAY_H

#include "grid.h"
#include "options.h"
#include "raylib.h"

typedef struct {
    float update_interval;
    float timer;
    Grid* grid;
} Conway;

Conway conway_new(float update_interval, Grid* grid);

Cell* conway_clicked(Conway* conway, const Vector2 clicked_pos);

void conway_update(Conway* conway, const float delta_time);

#endif  // CONWAY_H
