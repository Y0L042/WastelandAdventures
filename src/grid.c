#include "grid.h"
#include "collision.h"

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
            Color clr;
            int idx = grid_c2i(grid, x, y);
            if (grid_test_place(grid, grid->arr_coll_masks[idx], x, y) != 0)
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
    int idx = grid_c2i(grid, x_coord, y_coord);
    coll_bits_t grid_coll_layer = grid->arr_coll_masks[idx];
    int result = grid_coll_layer & coll_mask;

    //printf("grid index = %d \n", idx);
    //printf("move results = %d \n", grid_coll_layer);

    return (result);
}

coll_bits_t grid_get_coll_at(Grid *grid, int x_coord, int y_coord)
{
    int idx = grid_c2i(grid, x_coord, y_coord);

    return grid->arr_coll_masks[idx];
}

CVecVoid* grid_get_entities_at(Grid *grid, int x_coord, int y_coord)
{
    int idx = grid_c2i(grid, x_coord, y_coord);

    return &grid->arr_entity_refs[idx];
}

void grid_move_to(Grid *grid, int coll_layer, int x_coord, int y_coord)
{
   int idx = grid_c2i(grid, x_coord, y_coord);
   grid->arr_coll_masks[idx] = coll_bits_add_layer(
           grid->arr_coll_masks[idx], 
           coll_layer
        );
}

void grid_move_from(Grid *grid, int coll_layer, int x_coord, int y_coord)
{
   int idx = grid_c2i(grid, x_coord, y_coord);
   grid->arr_coll_masks[idx] = coll_bits_remove_layer(
           grid->arr_coll_masks[idx], 
           coll_layer
        );
}

// Coordinates to Index
int grid_c2i(Grid *grid, int x, int y)
{
    int idx = grid->height * y + x;
    return idx;
}

// Index to Coordinates
void grid_i2c(Grid *grid, int idx, int *x_coord, int *y_coord)
{
    *x_coord = (int)(idx % grid->width);
    *y_coord = (int)(idx / grid->width);
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
    *grid_arr_coll_masks = (int *)calloc(area, sizeof(int));
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



