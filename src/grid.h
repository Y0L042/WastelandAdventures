#ifndef GRID_H
#define GRID_H

#include "cvec.h"
#include <raylib.h>
#include "flecs.h"
#include "log.h"
#include "dray.h"

#define GRID_CREATE_2D(_grid_width, _grid_height, _grid_type) \
	((_grid_type **)_grid_create_2d(_grid_width, _grid_height, sizeof(_grid_type)))

#define GRID_FREE_2D(_grid_ptr) \
	do {\
		free(_grid_ptr[0]); \
		free(_grid_ptr); \
	} while (0)

typedef int coll_bits_t;

typedef struct Grid {
	ecs_world_t *world;
    int **arr_coll_layers;
    DRay **arr_entity_refs; // ecs_entity_t
	int width;
	int height;
	int tile_width;
	int tile_height;
} Grid;

typedef struct GridPosition {
	int x, y;
	Grid *grid;
	ecs_entity_t entity;
	int coll_layer, coll_mask;
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
        ecs_entity_t entity, 
		int coll_layer, int coll_mask
    );
int grid_test_place(Grid *grid, int coll_mask, int x_coord, int y_coord);
coll_bits_t grid_get_coll_at(Grid *grid, int x_coord, int y_coord);
DRay* grid_get_entities_at(Grid *grid, int x_coord, int y_coord);
void grid_move_to(Grid *grid, int coll_layer, int x_coord, int y_coord);
void grid_move_from(Grid *grid, int coll_layer, int x_coord, int y_coord);
void grid_pos_to_world_pos(
        Grid *grid, 
        int grid_x, 
        int grid_y, 
        int *world_x, 
        int *world_y
    );
int grid_test_outofbounds(Grid *grid, int x_coord, int y_coord);
/* Accepts grid, x, y, rad, and DRay of Vector2 */
void grid_get_coords_in_radius(Grid *grid, int x, int y, int rad, char mode, DRay *coords);

void gridposition_initialize(
		GridPosition *gp,
		int x, int y,
		Grid *grid,
		ecs_entity_t entity,
		int coll_layer, int coll_mask
    );
int gridposition_move(GridPosition *gp, int new_x, int new_y, int ignore_coll);
int gridposition_delete(GridPosition *gp);


void _grid_alloc_arr_coll_layers(
        int ***grid_arr_coll_layers, // Ptr to 2D arr
        int x_count,
        int y_count
    );
void _grid_alloc_arr_entity_refs(
        DRay ***grid_arr_entity_refs, // Ptr to 2D arr
        int x_count,
        int y_count
    );
        
static inline void **_grid_create_2d(int width, int height, size_t size)
{
    void **grid = calloc(width, sizeof(void *));
	if (!grid) { return NULL; }
	grid[0] = calloc(width * height, size);
	if (!grid[0]) { free(grid); return NULL; }
	for (int i = 1; i < width; i++)
	{
		grid[i] = (char *)grid[0] + i * height * size;
	}

	return grid;
}

#endif // GRID_H
