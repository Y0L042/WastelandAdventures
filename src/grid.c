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

GridComponentData* grid_create_gridcomponent(
        Grid *grid, 
        ecs_entity_t entity 
    )
{
    GridComponentData *gc_d = (GridComponentData *)malloc(sizeof(GridComponentData));
    gridcomponentdata_initialize(gc_d, grid, entity);
    log_debug("1\tDEBUG");
    ecs_set(grid->world, entity, GridComponent, { .gc_d = gc_d });
    log_debug("2\tDEBUG");

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
        int ***grid_arr_coll_masks,
        int x_count,
        int y_count
    )
{
    *grid_arr_coll_masks = (int **)malloc(sizeof(int *) * y_count);
    if (*grid_arr_coll_masks == NULL) { log_warn("MemAlloc failed!\n"); exit(1); }
    for (int y_idx = 0; y_idx < y_count; y_idx++)
    {
        (*grid_arr_coll_masks)[y_idx] = (int *)malloc(sizeof(int) * x_count);
        if ((*grid_arr_coll_masks)[y_idx] == NULL)
        {
            log_warn("MemAlloc failed!\n"); 
            exit(1); 
        }
    }
}

void _grid_alloc_arr_entity_refs(
        CVecVoid ***grid_arr_entity_refs,
        int x_count,
        int y_count
    )
{
    *grid_arr_entity_refs = (CVecVoid **)malloc(sizeof(CVecVoid *) * y_count);
    if (grid_arr_entity_refs == NULL) { log_warn("MemAlloc failed!\n"); exit(1); }
    for (int y_idx = 0; y_idx < y_count; y_idx++)
    {
        (*grid_arr_entity_refs)[y_idx] = (CVecVoid *)malloc(sizeof(CVecVoid) * x_count);
        if ((*grid_arr_entity_refs)[y_idx] == NULL)
        {
            log_warn("MemAlloc failed!\n"); 
            exit(1); 
        }
    }
}



