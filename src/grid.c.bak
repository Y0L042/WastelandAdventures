#include "grid.h"
#include "main.h"

void tile_initialize(
		Tile *tile, 
		ecs_world_t *world,
		int pos_x, 
		int pos_y, 
		int width, 
		int height,
		Color color
	)
{
	tile->uuid = uuid_generate();
	tile->world = world;
	tile->width = width;
	tile->height = height;
	tile->pos_x = pos_x;
	tile->pos_y = pos_y;
	tile->collision_layer = 0;
	cvec_void_init(&tile->gc_refs);	
}

void tile_free(Tile *tile)
{
	free(tile);
}

void tile_draw(Tile *tile)
{
	DrawRectangleLines(tile->pos_x, tile->pos_y, tile->width, tile->height, RAYWHITE);
}

int tile_test_for_collision(Tile *tile, ecs_ref_t *gc_ref)
{
	GridComponentData *gc_d = gc_ref_data(tile->world, gc_ref);

	return ((tile->collision_layer & gc_d->collision_layer) == 0); 
}

void tile_add_gc(Tile *tile, ecs_ref_t *gc_ref)
{
	if (tile_test_for_collision(tile, gc_ref) == 0)
	{
		cvec_void_add_item(&tile->gc_refs, gc_ref);
		GridComponentData *gc_d = gc_ref_data(tile->world, gc_ref);
		tile->collision_layer = tile->collision_layer | gc_d->collision_layer;
	}
	else
	{
		log_info("Tile: Entity collision conflict at { %d, %d }!\n", tile->pos_x, tile->pos_y); 
	}
}

void tile_remove_gc(Tile *tile, ecs_ref_t *gc_ref)
{
	for (int i = 0; i < tile->gc_refs.count; i++)
	{
		if (tile->gc_refs.data[i] == gc_ref)
		{
			cvec_void_clear_idx(&tile->gc_refs, i);
			GridComponentData *gc_d = gc_ref_data(tile->world, gc_ref);
			tile->collision_layer = tile->collision_layer - gc_d->collision_layer;
			break;
		}
	}
	cvec_void_defragment(&tile->gc_refs);
}



void grid_initialize(
        Grid *grid, 
        ecs_world_t *world, 
        int width, 
        int height, 
        int tile_width, 
        int tile_height
    )
{
	grid->uuid = uuid_generate();
	grid->world = world;
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
			tile_initialize(tile, grid->world, pos_x, pos_y, tile_width, tile_height, RAYWHITE);
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

GridComponentData* grid_create_gridcomponent(Grid *grid, ecs_entity_t entity)
{
	GridComponentData *gc_d = (GridComponentData *)malloc(sizeof(GridComponentData));
	gridcomponentdata_initialize(gc_d, grid, entity, 0, 0);

	ecs_set(grid->world, entity, GridComponent, { .gc_d = gc_d });
	ecs_ref_t *gc_ref = (ecs_ref_t *)malloc(sizeof(ecs_ref_t));
	*gc_ref = ecs_ref_init(grid->world, entity, GridComponent);
	gc_d->gc_ref = gc_ref;

    return gc_d;
}

Tile* grid_move_entity(
		Grid *grid, 
		ecs_ref_t *gc_ref,
		int old_x, int old_y,
		int new_x, int new_y
	)
{
	Tile *old_t = grid_get_tile_from_coords(grid, old_x, old_y);
	tile_remove_gc(old_t, gc_ref);
	
	Tile *new_t = grid_get_tile_from_coords(grid, new_x, new_y);
	tile_add_gc(new_t, gc_ref);
	
	return new_t;
}

Tile* grid_get_tile_from_coords(Grid *grid, int x, int y)
{
    Tile *t = grid->tiles[x][y];
    return t;
}

void gridcomponentdata_initialize(
		GridComponentData *gc_d, 
		Grid *grid, 
		ecs_entity_t entity,
		int pos_x, 
		int pos_y
	)
{
    gc_d->grid = grid;
    gc_d->entity = entity;
}

void gridcomponent_initialize(GridComponent *gc, GridComponentData *gc_d)
{
	gc->gc_d = gc_d;
}

GridComponentData* gc_ref_data(ecs_world_t *world, ecs_ref_t *gc_ref)
{
	const GridComponent *gc = ecs_ref_get(world, gc_ref, GridComponent);
	GridComponentData *gc_d = gc->gc_d;

	return gc_d;
}
