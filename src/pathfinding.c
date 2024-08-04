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

CVecInt* pathmap_find_path(
        PathMap *pm, 
        int x_a, 
        int y_a, 
        int x_b, 
        int y_b,
        coll_bits_t coll_mask
    )
{
    return _pathmap_find_path_BFS(
            pm,
            x_a,
            y_a,
            x_b,
            y_b,
            coll_mask
        );
}

CVecInt* _pathmap_find_path_BFS(
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
    CVecInt cell_parent;
    cvec_int_init(&cell_parent);
    
    int start_idx = grid_c2i(pm->grid, x_a, y_a);
    int target_idx = grid_c2i(pm->grid, x_b, y_b);

    cvec_int_add_item(&cell_queue, start_idx);
    cvec_int_add_item(&cell_visited, start_idx);
    cvec_int_add_item(&cell_parent, -1); // start cell has no parent

    while (cell_queue.count != 0)
    {
        int idx = cell_queue.data[0];
        cvec_int_remove_idx(&cell_queue, 0);
        int curr_cell_x, curr_cell_y;
        grid_i2c(pm->grid, idx, &curr_cell_x, &curr_cell_y);

        if (idx == target_idx)
        {
            CVecInt *path = (CVecInt *)malloc(sizeof(CVecInt));
            cvec_int_init(path);

            int path_idx = idx;
            while (path_idx != -1)
            {
                cvec_int_add_item(path, path_idx);
                for (int i = 0; i < cell_visited.count; i++)
                {
                    if (cell_visited.data[i] == path_idx)
                    {
                        path_idx = cell_parent.data[i];
                        break;
                    }
                }
            }

            cvec_int_free(&cell_queue);
            cvec_int_free(&cell_visited);
            cvec_int_free(&cell_parent);

            return path;
        }

        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                if (i == 0 && j == 0) { continue; }

                int cell_x, cell_y;
                cell_x = curr_cell_x + i;
                cell_y = curr_cell_y +j;

                if (cell_x < 0 || cell_x >= pm->grid->width){ continue; }
                if (cell_y < 0 || cell_y >= pm->grid->height){ continue; }

                int neighbor_idx = grid_c2i(pm->grid, cell_x, cell_y);
                int visited = _pathmap_is_cell_visited(&cell_visited, neighbor_idx);
                
                if (visited) { continue; }
                if ((*pm->coll_map_ref)[neighbor_idx] & coll_mask != 0)
                { continue; }

                cvec_int_add_item(&cell_queue, neighbor_idx);
                cvec_int_add_item(&cell_visited, neighbor_idx);
                cvec_int_add_item(&cell_parent, idx);
            }
        }
    }

    cvec_int_free(&cell_queue);
    cvec_int_free(&cell_visited);
    cvec_int_free(&cell_parent);

    return NULL;
}

int _pathmap_is_cell_visited(CVecInt *visited_cells, int cell_idx)
{
    int visited = 0;
    for (int i = 0; i < visited_cells->count; i++)
    {
        if (visited_cells->data[i] == cell_idx)
        {
            visited = 1;
            break;
        }
    }

    return visited;
}

