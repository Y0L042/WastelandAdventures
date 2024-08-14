#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include "grid.h"
#include "dray.h"

DRay* mapgen_generate_RANDOMWALKER(Grid *grid);
DRay* mapgen_get_neighbor_idx(Grid *grid, int idx);

#endif // MAP_GENERATOR_H
