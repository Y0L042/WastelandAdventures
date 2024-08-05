#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "cvec.h"

#include <raylib.h>
#include "collision.h"
#include "grid.h"

typedef struct PathMap {
    Grid *grid;
    int **coll_map_ref; // NO TOUCHY!
    CVecVoid **entity_map_ref; // NO TOUCHY!
} PathMap;

typedef struct PathComponent {
    PathMap *pm;
} PathComponent;

void pathmap_initialize(
        PathMap *pm,
        Grid *grid
    );
CVecInt* pathmap_find_path(
        PathMap *pm, 
        int x_a, 
        int y_a, 
        int x_b, 
        int y_b,
        coll_bits_t coll_mask
    );
CVecInt* _pathmap_find_path_ASTAR(
        PathMap *pm, 
        int x_a, 
        int y_a, 
        int x_b, 
        int y_b,
        coll_bits_t coll_mask
    );
CVecInt* _pathmap_find_path_BFS(
        PathMap *pm, 
        int x_a, 
        int y_a, 
        int x_b, 
        int y_b,
        coll_bits_t coll_mask
    );
int _pathmap_is_cell_visited(CVecInt *visited_cells, int cell_idx);

#endif // PATHFINDING_H
