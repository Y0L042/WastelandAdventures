#ifndef GRID_H
#define GRID_H

#include "flecs.h"

#include "uuid.h"
#include "cvec.h"
#include "glyph.h"

#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>
#include <stdint.h>

typedef struct Tile {
	UUID uuid;
	ecs_world_t *world;
	int width;
	int height;
	int pos_x; // TOP LEFT corner
	int pos_y; // TOP LEFT corner
    CVecVoid gc_refs; // CVecVoid<ecs_entity_t> entities
    uint16_t collision_layer;
} Tile;

typedef struct Grid {
	UUID uuid;
	ecs_world_t *world;
	Tile ***tiles;
	int width;
	int height;
	int tile_width;
	int tile_height;
} Grid;

typedef struct GridComponentData {
    Grid *grid;
    Tile *tile;
	uint16_t collision_layer;
	ecs_entity_t entity;
	ecs_ref_t *gc_ref; // GridComponent *
} GridComponentData;

typedef struct GridComponent {
	GridComponentData *gc_d;
} GridComponent;

void tile_initialize(
		Tile *tile, 
		ecs_world_t *world,
		int pos_x, 
		int pos_y, 
		int width, 
		int height,
		Color color
	);
void tile_free(Tile *tile);
void tile_draw(Tile *tile);
int tile_test_for_collision(Tile *tile, ecs_ref_t *gc_ref);
void tile_add_gc(Tile *tile, ecs_ref_t *gc_ref);
void tile_remove_gc(Tile *tile, ecs_ref_t *gc_ref);

void grid_initialize(Grid *grid, ecs_world_t *world, int width, int height, int tile_width, int tile_height);
void grid_initialize_tiles(Grid *grid, int tile_width, int tile_height);
void grid_free(Grid *grid);
void grid_draw(Grid *grid);
GridComponentData* grid_create_gridcomponent(Grid *grid, ecs_entity_t entity);
Tile* grid_move_entity(
		Grid *grid, 
		ecs_ref_t *gc_ref,
		int old_x, int old_y,
		int new_x, int new_y
	);
Tile* grid_get_tile_from_coords(Grid *grid, int x, int y);

void gridcomponentdata_initialize(
		GridComponentData *gc_d, 
		Grid *grid, 
		int pos_x, 
		int pos_y
	);

void gridcomponent_initialize(GridComponent *gc, GridComponentData *gc_d);
GridComponentData* gc_ref_data(ecs_world_t *world, ecs_ref_t *gc_ref);

#endif // GRID_H
