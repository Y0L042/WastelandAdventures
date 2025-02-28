#include "grid.h"

#include "log.h"

#include <raylib.h>

/* --- Private Functions --- */
static DRay **grid_alloc_arr_entity_refs(
        int x_count,
        int y_count
    );
static unsigned int **grid_alloc_arr_coll_layers(
        int x_count,
        int y_count
    );

Grid *grid_create(ecs_world_t *world, 
                     int width, 
                     int height, 
                     int tile_width, 
                     int tile_height)
{
    Grid *grid = (Grid *)malloc(sizeof(Grid));
    grid->world = world;

    grid->sizes = (GridSizes){ .width = width,
                        .height = height,
                        .tile_width = tile_width,
                        .tile_height = tile_height };

    GridData *data = (GridData *)malloc(sizeof(GridData));
    data->entity_grid = grid_alloc_arr_entity_refs(width, height);
    data->coll_layers_grid = grid_alloc_arr_coll_layers(width, height);


    return grid;
}

void grid_draw(Grid *grid)
{
    int width, height, tile_width, tile_height;
    width = grid->sizes.width;
    height = grid->sizes.height;
    tile_width = grid->sizes.tile_width;
    tile_height = grid->sizes.tile_height;

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            DrawRectangleLines(
                    i * tile_width - (tile_width / 2.0), 
                    j * tile_height - (tile_height / 2.0),
                    tile_width,
                    tile_height,
                    RAYWHITE
            );
            DrawCircle(
                    i * tile_width,
                    j * tile_height,
                    1.0f,
                    GREEN
            );
        }
    }
}

void grid_print_error(GridErrors error)
{
    switch (error) {
        case GRID_SERIALIZATION_SUCCESSFUL:
            log_debug("Grid Serialization Successful.");
            break;
        default:
            log_debug("Unknown grid error!");
            break;
    }
}

static unsigned int **grid_alloc_arr_coll_layers(
        int x_count,
        int y_count
    )
{
    unsigned int **grid_arr_coll_layers = 
        (unsigned int **)calloc(x_count, sizeof(unsigned int *));
	(grid_arr_coll_layers)[0] = 
        (unsigned int *)calloc(x_count * y_count, sizeof(unsigned int));
	for (int i = 1; i < x_count; i++)
	{
		(grid_arr_coll_layers)[i] = (grid_arr_coll_layers[0]) + i * y_count;
	}

    return grid_arr_coll_layers;
}

static DRay **grid_alloc_arr_entity_refs(
        int x_count,
        int y_count
    )
{
    DRay **grid_arr_entity_refs = (DRay **)malloc(x_count * sizeof(DRay *));
	(grid_arr_entity_refs)[0] = 
        (DRay *)malloc(x_count * y_count * sizeof(DRay));
    for (int i = 1; i < x_count; i++)
    {
		(grid_arr_entity_refs)[i] = (grid_arr_entity_refs)[0] + i * y_count;
    }
	for (int x = 0; x < x_count; x++)
	{
		for (int y = 0; y < y_count; y++)
		{
			DRAY_INIT_VALUES(&(grid_arr_entity_refs)[x][y], ecs_entity_t);
		}
	}

    return grid_arr_entity_refs;
}
