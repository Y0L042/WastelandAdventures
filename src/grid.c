#include "components.h"

void grid_initialize(
        Grid *grid, 
        ecs_world_t *world, 
        int width, 
        int height, 
        int tile_width, 
        int tile_height
    )
{
    grid->world = world;
    grid->width = width;
    grid->height = height;
    grid->tile_width = tile_width;
    grid->tile_height = tile_height;

    _grid_alloc_arr_coll_layers(&grid->arr_coll_layers, width, height);
    _grid_alloc_arr_entity_refs(&grid->arr_entity_refs, width, height);
}

void grid_free(Grid *grid)
{
    free(grid);
}

void grid_draw(Grid *grid)
{
    for (int y = 0; y < grid->height; y++)
    {
        for (int x = 0; x < grid->width; x++)
        {
            Color clr;
            if (grid_test_place(grid, grid->arr_coll_layers[x][y], x, y) != 0)
            {
                clr = RED;
            }
            else
            {
                clr = RAYWHITE;
            }
            DrawRectangleLines(
                    grid->tile_width * x, 
                    grid->tile_height * y, 
                    grid->tile_width, 
                    grid->tile_height, 
                    clr
                );
        }
    }
}

int grid_test_place(Grid *grid, coll_bits_t coll_mask, int x_coord, int y_coord)
{
    coll_bits_t grid_coll_layer = grid->arr_coll_layers[x_coord][y_coord];
    int result = grid_coll_layer & coll_mask;

    return (result);
}

coll_bits_t grid_get_coll_at(Grid *grid, int x_coord, int y_coord)
{
    return grid->arr_coll_layers[x_coord][y_coord];
}

DRay* grid_get_entities_at(Grid *grid, int x_coord, int y_coord)
{
    return &grid->arr_entity_refs[x_coord][y_coord];
}

void grid_move_to(Grid *grid, int coll_layer, int x_coord, int y_coord)
{
   //int idx = grid_c2i(grid, x_coord, y_coord);
   //grid->arr_coll_layers[idx] = coll_bits_add_layer(
   //        grid->arr_coll_layers[idx], 
   //        coll_layer
   //     );
}

void grid_move_from(Grid *grid, int coll_layer, int x_coord, int y_coord)
{
   //grid->arr_coll_layers[idx] = coll_bits_remove_layer(
   //        grid->arr_coll_layers[idx], 
   //        coll_layer
   //     );
}

void grid_pos_to_world_pos(
        Grid *grid, 
        int grid_x, 
        int grid_y, 
        int *world_x, 
        int *world_y
    )
{
    *world_x = grid->tile_width * grid_x;
    *world_y = grid->tile_height * grid_y;
}

int grid_test_outofbounds(Grid *grid, int x_coord, int y_coord)
{
    if (x_coord < 0 || x_coord >= grid->width)
    {
        return 1;
    }
    if (y_coord < 0 || y_coord >= grid->height)
    {
        return -1;
    }

    return 0;
}

void grid_create_gridposition(
		int x, int y,
        Grid *grid, 
        ecs_entity_t entity, 
		int coll_layer,
		int coll_mask
    )
{
    ecs_set(grid->world, entity, GridPosition, {
				.x = x,
				.y = y,
				.grid = grid,
				.entity = entity,
				.coll_layer = coll_layer,
				.coll_mask = coll_mask
			});
	grid->arr_coll_layers[x][y] = grid->arr_coll_layers[x][y] | coll_layer;
	dray_add_value(&grid->arr_entity_refs[x][y], entity, ecs_entity_t);
}

void gridposition_initialize(
		GridPosition *gp,
		int x, int y,
		Grid *grid,
		ecs_entity_t entity,
		int coll_layer, int coll_mask
	)
{
	gp->x = x;
	gp->y = y;
	gp->grid = grid;
	gp->entity = entity;
}

int gridposition_move(
		GridPosition *gp, 
		int new_x, 
		int new_y, 
		int ignore_coll
	)
{
	Grid *grid = gp->grid;
	if (new_x < 0 || new_x >= grid->width) { return 1; }
	if (new_y < 0 || new_y >= grid->height) { return 1; }

	int coll_layer = gp->coll_layer;
	int coll_mask = gp->coll_mask;

	if (ignore_coll == 0)
	{
		coll_bits_t grid_coll_layer = grid->arr_coll_layers[new_x][new_y];
		int result = grid_coll_layer & coll_mask;
		if (result != 0) { return 1;}

		/* Remove layer */
		grid->arr_coll_layers[gp->x][gp->y] = grid->arr_coll_layers[gp->x][gp->y] & (~coll_layer);
		for (int i = 0; i < grid->arr_entity_refs[gp->x][gp->y].count; i++)
		{
			if (dray_get_value(&grid->arr_entity_refs[gp->x][gp->y], i, ecs_entity_t) == gp->entity)
			{
				dray_remove_idx(&grid->arr_entity_refs[gp->x][gp->y], i);
				break;
			}
		}

		/* Add layer */
	   	grid->arr_coll_layers[new_x][new_y] = grid->arr_coll_layers[new_x][new_y] | coll_layer;
		dray_add_value(&grid->arr_entity_refs[new_x][new_y], gp->entity, ecs_entity_t);
	}

	gp->x = new_x;
	gp->y = new_y;

	return 0;
}

int gridposition_delete(GridPosition *gp)
{
	/* Remove layer */
	int x = gp->x;
	int y = gp->y;
	Grid *grid = gp->grid;
	int coll_layer = gp->coll_layer;
	grid->arr_coll_layers[x][y] = grid->arr_coll_layers[x][y] & (~coll_layer);
	for (int i = 0; i < grid->arr_entity_refs[x][y].count; i++)
	{
		if (dray_get_value(&grid->arr_entity_refs[x][y], i, ecs_entity_t) == gp->entity)
		{
			dray_remove_idx(&grid->arr_entity_refs[x][y], i);
			break;
		}
	}
}

/*
short **allocGrid() {
    short i;
    short **array = malloc(DCOLS * sizeof(short *));

    array[0] = malloc(DROWS * DCOLS * sizeof(short));
    for(i = 1; i < DCOLS; i++) {
        array[i] = array[0] + i * DROWS;
    }
    return array;
}

void freeGrid(short **array) {
    free(array[0]);
    free(array);
}

void copyGrid(short **to, short **from) {
    short i, j;

    for(i = 0; i < DCOLS; i++) {
        for(j = 0; j < DROWS; j++) {
            to[i][j] = from[i][j];
        }
    }
}

void fillGrid(short **grid, short fillValue) {
    short i, j;

    for(i = 0; i < DCOLS; i++) {
        for(j = 0; j < DROWS; j++) {
            grid[i][j] = fillValue;
        }
    }
}
*/

void _grid_alloc_arr_coll_layers(
        int ***grid_arr_coll_layers,
        int x_count,
        int y_count
    )
{
    *grid_arr_coll_layers = (int **)calloc(x_count, sizeof(int *));
	(*grid_arr_coll_layers)[0] = (int *)calloc(x_count * y_count, sizeof(int));
	for (int i = 1; i < x_count; i++)
	{
		(*grid_arr_coll_layers)[i] = (*grid_arr_coll_layers[0]) + i * y_count;
	}
}

void _grid_alloc_arr_entity_refs(
        DRay ***grid_arr_entity_refs,
        int x_count,
        int y_count
    )
{
    *grid_arr_entity_refs = (DRay **)malloc(x_count * sizeof(DRay *));
	(*grid_arr_entity_refs)[0] = (DRay *)malloc(x_count * y_count * sizeof(DRay));
    for (int i = 1; i < x_count; i++)
    {
		(*grid_arr_entity_refs)[i] = (*grid_arr_entity_refs)[0] + i * y_count;
    }
	for (int x = 0; x < x_count; x++)
	{
		for (int y = 0; y < y_count; y++)
		{
			dray_init_values(&(*grid_arr_entity_refs)[x][y], ecs_entity_t);
		}
	}
}



