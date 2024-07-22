#ifndef GRID_H
#define GRID_H

#include "uuid.h"
#include "cvec.h"
#include "glyph.h"

#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>
#include <stdint.h>

typedef struct Tile {
	UUID uuid;
	int width;
	int height;
	int pos_x; // TOP LEFT corner
	int pos_y; // TOP LEFT corner
    CVecVoid entities; // CVecVoid<ecs_entity_t> entities
    uint16_t collision_layer;
} Tile;

void tile_initialize(
		Tile *tile, 
		int pos_x, 
		int pos_y, 
		int width, 
		int height,
		Color color
	);
void tile_free(Tile *tile);
void tile_draw(Tile *tile);

typedef struct Grid {
	UUID uuid;
	Tile ***tiles;
	int width;
	int height;
	int tile_width;
	int tile_height;
} Grid;

typedef struct GridComponent {
    Grid *grid;
    Tile *tile;
} GridComponent;

void grid_initialize(Grid *grid, int width, int height, int tile_width, int tile_height);
void grid_initialize_tiles(Grid *grid, int tile_width, int tile_height);
void grid_free(Grid *grid);
void grid_draw(Grid *grid);
Tile* grid_get_tile_from_coords(Grid *grid, int x, int y);
//Vector2 grid_get_coords_from_tile(Grid *grid

void gridcomponent_initialize(GridComponent *gc, Grid *grid, int pos_x, int pos_y);

#endif // GRID_H
