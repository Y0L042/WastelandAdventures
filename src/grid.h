#ifndef GRID_H
#define GRID_H

#include <stdint.h>

#include <flecs.h>
#include "dray.h"

typedef enum GridErrors {
    GRID_SERIALIZATION_SUCCESSFUL
} GridErrors;

typedef uint32_t coll_bits_t;



/* --- Grid Structs --- */
typedef struct GridData {
    coll_bits_t **coll_layers_grid; 
    DRay **entity_grid /* <ecs_entity_t> */;
} GridData;

typedef struct GridSizes {
    int width, height, tile_width, tile_height;
} GridSizes;

typedef struct Grid {
    ecs_world_t *world;
    GridData data;
    GridSizes sizes;
} Grid;



/* --- Grid Components --- */
typedef struct GridPosition {
    int x, y;
} GridPosition;

typedef struct GridVelocity {
    int x, y;
} GridVelocity;

typedef struct GridCollision {
    coll_bits_t coll_layer, coll_mask;
} GridCollision;

typedef struct GridRefs {
    ecs_entity_t entity;
    Grid *grid;
} GridRefs;



Grid *grid_create(ecs_world_t *world, 
                     int width, 
                     int height, 
                     int tile_width, 
                     int tile_height);
void grid_draw(Grid *grid);
void grid_print_error(GridErrors error);

#endif /* GRID_H */
