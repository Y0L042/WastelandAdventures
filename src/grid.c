#include "grid.h"

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

    _grid_alloc_arr_coll_masks(&grid->arr_coll_masks, width, height);
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
            DrawRectangleLines(
                    grid->tile_width * x, 
                    grid->tile_height * y, 
                    grid->tile_width, 
                    grid->tile_height, 
                    RAYWHITE
                );
        }
    }
}

int grid_test_move(Grid *grid, coll_bits_t coll_mask, int x_coord, int y_coord)
{
    int idx = grid_c2i(grid, x_coord, y_coord);
    coll_bits_t grid_coll_layer = grid->arr_coll_masks[idx];

    return (grid_coll_layer & coll_mask);
}

// Coordinates to Index
int grid_c2i(Grid *grid, int x, int y)
{
    int idx = grid->width * y + x;

    return idx;
}

// Index to Coordinates
void grid_i2c(Grid *grid, int idx, int *x_coord, int *y_coord)
{
    *x_coord = (int)(idx % grid->width);
    *y_coord = (int)(idx / grid->width);
}



GridComponentData* grid_create_gridcomponent(
        Grid *grid, 
        ecs_entity_t entity 
    )
{
    GridComponentData *gc_d = (GridComponentData *)malloc(sizeof(GridComponentData));
    gridcomponentdata_initialize(gc_d, grid, entity);
    ecs_set(grid->world, entity, GridComponent, { .gc_d = gc_d });

    return gc_d;
}



void gridcomponentdata_initialize(
        GridComponentData *gc_d,
        Grid *grid,
        ecs_entity_t entity
    )
{
   gc_d->grid = grid;
   gc_d->entity = entity;
}



void _grid_alloc_arr_coll_masks(
        int **grid_arr_coll_masks,
        int x_count,
        int y_count
    )
{
    long int area = ((long int)x_count) * ((long int)y_count);
    *grid_arr_coll_masks = (int *)malloc(sizeof(int) * area);
}

void _grid_alloc_arr_entity_refs(
        CVecVoid **grid_arr_entity_refs,
        int x_count,
        int y_count
    )
{
    long int area = ((long int)x_count) * ((long int)y_count);
    *grid_arr_entity_refs = (CVecVoid *)malloc(sizeof(CVecVoid) * area);
}



