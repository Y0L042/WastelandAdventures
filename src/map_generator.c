#include "map_generator.h"
#include "maths.h"

DRay* mapgen_generate_RANDOMWALKER(Grid *grid)
{
    DRay *visited = (DRay *)malloc(sizeof(DRay));
    dray_init_values(visited, Vector2);

    int start_x = maths_randbetween_int(0, grid->width - 1); 
	int start_y = maths_randbetween_int(0, grid->height - 1);
	Vector2 val = { start_x, start_y };
    dray_add_value(visited, val, Vector2); 
     
	int x = start_x;
	int y = start_y;
    int counter = 0;
    while (counter++ < 1000)
    {
        DRay *neighbors = mapgen_get_neighbor_coord(grid, x, y);
		if (neighbors->count == 0) 
		{
			free(neighbors);
			continue; 
		}
		int rand_idx = maths_randbetween_int(0, neighbors->count - 1);
        Vector2 rand_neighbor = dray_get_value(neighbors, rand_idx, Vector2);
		dray_add_value(visited, rand_neighbor, Vector2);
		x = rand_neighbor.x;
		y = rand_neighbor.y;
		free(neighbors);
    }

    return visited;
}

DRay* mapgen_get_neighbor_coord(Grid *grid, int x, int y)
{
    DRay *neighbors = (DRay *)malloc(sizeof(DRay));
    dray_init_values(neighbors, Vector2);
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
			/* Skip same cell */
            if (i == 0 && j == 0) 
            { continue; }

			/* Skip out of bounds */
            if ((x + i) < 0 || (x + i) >= grid->width)
            { continue; }
            if ((y + j) < 0 || (y + j) >= grid->height)
            { continue; }
			Vector2 val = { x + i, y + j };
            dray_add_value(neighbors, val, Vector2);
        }
    }
    return neighbors;
}


