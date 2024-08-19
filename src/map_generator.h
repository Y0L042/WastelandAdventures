#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include <raylib.h>
#include "grid.h"
#include "dray.h"

DRay* mapgen_generate_RANDOMWALKER(Grid *grid);
DRay* mapgen_get_neighbor_coord(Grid *grid, int x, int y);

#endif // MAP_GENERATOR_H
