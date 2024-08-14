#ifndef GRID_H
#define GRID_H

#include "cvec.h"
#include <raylib.h>
#include "flecs.h"
#include "log.h"
#include "dray.h"

typedef int coll_bits_t;

typedef struct Grid {
	ecs_world_t *world;
    int *arr_coll_masks;
    DRay *arr_entity_refs;
	int width;
	int height;
	int tile_width;
	int tile_height;
} Grid;

typedef struct GridPosition {
	int x, y;
	Grid *grid;
	ecs_entity_t entity;
} GridPosition;

void grid_initialize(
        Grid *grid, 
        ecs_world_t *world, 
        int width, 
        int height, 
        int tile_width, 
        int tile_height
    );
void grid_free(Grid *grid);
void grid_draw(Grid *grid);
void grid_create_gridposition(
		int x, int y,
        Grid *grid, 
        ecs_entity_t entity 
    );
int grid_test_place(Grid *grid, int coll_mask, int x_coord, int y_coord);
coll_bits_t grid_get_coll_at(Grid *grid, int x_coord, int y_coord);
DRay* grid_get_entities_at(Grid *grid, int x_coord, int y_coord);
void grid_move_to(Grid *grid, int coll_layer, int x_coord, int y_coord);
void grid_move_from(Grid *grid, int coll_layer, int x_coord, int y_coord);
int grid_c2i(Grid *grid, int x, int y);
void grid_i2c(Grid *grid, int idx, int *x_coord, int *y_coord);
void grid_pos_to_world_pos(
        Grid *grid, 
        int grid_x, 
        int grid_y, 
        int *world_x, 
        int *world_y
    );
int grid_test_outofbounds(Grid *grid, int x_coord, int y_coord);

void gridposition_initialize(
        GridPosition *gp,
		int x, int y,
        Grid *grid,
        ecs_entity_t entity
    );
int gridposition_move(GridPosition *gp, int new_x, int new_y);

void _grid_alloc_arr_coll_masks(
        int **grid_arr_coll_masks, // Ptr to 2D arr
        int x_count,
        int y_count
    );
void _grid_alloc_arr_entity_refs(
        DRay **grid_arr_entity_refs, // Ptr to 2D arr
        int x_count,
        int y_count
    );
        


#endif // GRID_H
