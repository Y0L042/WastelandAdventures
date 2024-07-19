#include "grid.h"
#include "main.h"

void tile_initialize(
		Tile *tile, 
		int pos_x, 
		int pos_y, 
		int width, 
		int height,
		//Glyph *glyph,
		Color color
	)
{
	tile->uuid = uuid_generate();
	tile->width = width;
	tile->height = height;
	tile->pos_x = pos_x;
	tile->pos_y = pos_y;
}

void tile_free(Tile *tile)
{
	free(tile);
}

void tile_draw(Tile *tile)
{
	DrawRectangleLines(tile->pos_x, tile->pos_y, tile->width, tile->height, RAYWHITE);
}




void grid_initialize(Grid *grid, int width, int height, int tile_width, int tile_height)
{
	grid->uuid = uuid_generate();

	grid->width = width;
	grid->height = height;
	grid->tile_width = tile_width;
	grid->tile_height = tile_height;

	// Allocate rows
	grid->tiles = (Tile ***)malloc(sizeof(Tile **) * height);
	if (grid->tiles == NULL)
	{
		fprintf(stderr, "Memory allocation failed!\n");
		exit(1);
	}

	// Allocate columns
	for (int i = 0; i < height; i++)
	{
		grid->tiles[i] = (Tile **)malloc(sizeof(Tile *) * width);
		if (grid->tiles[i] == NULL)
		{
			fprintf(stderr, "Memory allocation failed!\n");
			exit(1);
		}
	}

	grid_initialize_tiles(grid, tile_width, tile_height);
}

void grid_free(Grid *grid)
{
	for (int i = 0; i < grid->height; i++)
	{
		free(grid->tiles[i]);
	}
	free(grid->tiles);
}

void grid_initialize_tiles(Grid *grid, int tile_width, int tile_height)
{
	for (int y = 0; y < grid->height; y++)
	{
		for (int x = 0; x < grid->width; x++)
		{
			int pos_x = x * tile_width;
			int pos_y = y * tile_height;

			Tile *tile = (Tile *)malloc(sizeof(Tile));
			tile_initialize(tile, pos_x, pos_y, tile_width, tile_height, RAYWHITE);
			grid->tiles[x][y] = tile;
		}
	}
}

void grid_draw(Grid *grid)
{
	for (int y = 0; y < grid->height; y++)
	{
		for (int x = 0; x < grid->width; x++)
		{
			tile_draw(grid->tiles[x][y]);
		}
	}
}
