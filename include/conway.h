#ifndef CONWAY_IMPL
#define CONWAY_IMPL

#include "grid.h"
#include "raylib.h"

typedef struct {
    float update_interval;
    float timer;
    Grid* grid;
} Conway;

Conway conway_new(float update_interval, Grid* grid);

void conway_clicked(Conway* conway, const Vector2* clicked_pos);

void conway_update(Conway* conway, const float delta_time);

#endif  // CONWAY_IMPL
