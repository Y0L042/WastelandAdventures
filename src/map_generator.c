#include "map_generator.h"
#include "maths.h"

DRay* mapgen_generate_RANDOMWALKER(Grid *grid)
{
    DRay *visited = (DRay *)malloc(sizeof(DRay));
    dray_init_values(visited, int);
    int area = grid->width * grid->height;

    int start_idx = maths_randbetween_int(0, area); 
    int start_x, start_y;
    grid_i2c(grid, start_idx, &start_x, &start_y);
    dray_add_value(visited, start_idx, int); 
     
    int idx = start_idx;
    int counter = 0;
    while (counter++ < 100)
    {
        DRay *neighbors = mapgen_get_neighbor_idx(grid, idx);
        int rand_neighbor = maths_randbetween_int(0, neighbors->count);
        dray_add_value(visited, rand_neighbor, int);
        idx = rand_neighbor;
    }

    return visited;
}

DRay* mapgen_get_neighbor_idx(Grid *grid, int idx)
{
    DRay *neighbors = (DRay *)malloc(sizeof(DRay));
    dray_init_values(neighbors, int);
    int x, y;
    grid_i2c(grid, idx, &x, &y);
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if (i == 0 && j == 0) 
            { continue; }
            if ((x + i) < 0 || (x + i) >= grid->tile_width)
            { continue; }
            if ((y + j) < 0 || (y +j) >= grid->height)
            { continue; }

            int idx = grid_c2i(grid, x + i, y + j);
            dray_add_value(neighbors, idx, int);
        }
    }
    return neighbors;
}


