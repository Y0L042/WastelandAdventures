#include "pathfinding.h"

void pathmap_initialize(
        PathMap *pm,
        Grid *grid
    )
{
    pm->grid = grid;
    pm->coll_map_ref = &grid->arr_coll_masks;
    pm->entity_map_ref = &grid->arr_entity_refs;
}

CVecVoid* pathmap_find_path(
        PathMap *pm, 
        int x_a, 
        int y_a, 
        int x_b, 
        int y_b,
        coll_bits_t coll_mask
    )
{
    return NULL;
}

/*
 *   BFS
 *
 *   1. Add root node to the queue, and mark it as visited(already explored).
 *   2. Loop on the queue as long as it's not empty.
 *      1. Get and remove the node at the top of the queue(current).
 *      2. For every non-visited child of the current node, do the following: 
 *          1. Mark it as visited.
 *          2. Check if it's the goal node, If so, then return it.
 *          3. Otherwise, push it to the queue.
 *   3. If queue is empty, then goal node was not found!
*/
CVecVoid* _pathmap_find_path_BFS(
        PathMap *pm, 
        int x_a, 
        int y_a, 
        int x_b, 
        int y_b,
        coll_bits_t coll_mask
    )
{
    CVecInt cell_queue;
    cvec_int_init(&cell_queue);
    CVecInt cell_visited;
    cvec_int_init(&cell_visited);
    
    int start_idx = grid_c2i(pm->grid, x_a, y_a);
    cvec_int_add_item(&cell_queue, start_idx);
    cvec_int_add_item(&cell_visited, start_idx);

    while (cell_queue.count != 0)
    {
        int idx = cell_queue.data[0];
        cvec_int_remove_idx(&cell_queue, 0);
        int curr_cell_x, curr_cell_y;
        grid_i2c(pm->grid, idx, &curr_cell_x, &curr_cell_y);

        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                int cell_x, cell_y;
                cell_x = curr_cell_x + i;
                cell_y = curr_cell_y +j;

                if (cell_x < 0 || cell_x >= pm->grid->width)
                {
                    continue;
                }
                if (cell_y < 0 || cell_y >= pm->grid->height)
                {
                    continue;
                }



            }
        }
    }

    return NULL;
}

int _pathmap_is_tile_valid(
        PathMap *pm,
        int x_tile,
        int y_tile,
        coll_bits_t coll_mask
    )
{

}
