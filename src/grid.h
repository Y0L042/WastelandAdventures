#ifndef GRID_H
#define GRID_H

#include "cvec.h"
#include <raylib.h>
#include "flecs.h"
#include "log.h"
#include "components.h"

typedef struct Grid {
	ecs_world_t *world;
    int **arr_coll_masks;
    CVecVoid **arr_entity_refs;
	int width;
	int height;
	int tile_width;
	int tile_height;
} Grid;

typedef struct GridComponentData {
    Grid *grid;
	ecs_entity_t entity;
} GridComponentData;

typedef struct GridComponent {
	GridComponentData *gc_d;
} GridComponent;



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
GridComponentData* grid_create_gridcomponent(
        Grid *grid, 
        ecs_entity_t entity 
    );



void gridcomponentdata_initialize(
        GridComponentData *gc_d,
        Grid *grid,
        ecs_entity_t entity
    );



void _grid_alloc_arr_coll_masks(
        int ***grid_arr_coll_masks, // Ptr to 2D arr
        int x_count,
        int y_count
    );
void _grid_alloc_arr_entity_refs(
        CVecVoid ***grid_arr_entity_refs, // Ptr to 2D arr
        int x_count,
        int y_count
    );
        


#endif // GRID_H
