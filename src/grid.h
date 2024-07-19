#ifndef GRID_H
#define GRID_H

#include "uuid.h"
#include "cvec.h"
#include "glyph.h"

#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>

typedef struct Tile {
	UUID uuid;
	int width;
	int height;
	int pos_x; // TOP LEFT corner
	int pos_y; // TOP LEFT corner
	Glyph glyph;
	Color color;
} Tile;

void tile_initialize(
		Tile *tile, 
		int pos_x, 
		int pos_y, 
		int width, 
		int height,
		//Glyph *glyph,
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

void grid_initialize(Grid *grid, int width, int height, int tile_width, int tile_height);
void grid_initialize_tiles(Grid *grid, int tile_width, int tile_height);
void grid_free(Grid *grid);
void grid_draw(Grid *grid);

#endif // GRID_H
