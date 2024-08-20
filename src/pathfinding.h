#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <raylib.h>
#include "grid.h"
#include "flecs.h"
#include "DRay.h"

extern ECS_COMPONENT_DECLARE(PathComponent);

typedef struct PathComponent {
	Grid *grid;
} PathComponent;

static inline int cmp_add_PathComponent(
		ecs_world_t *world,
		ecs_entity_t entity,
		Grid *grid
	)
{
	ecs_set(world, entity, PathComponent, { .grid = grid });	

	return 0;
}

DRay* find_path(Grid *grid, int from_x, int from_y, int to_x, int to_y, int coll_mask);

#endif // PATHFINDING_H
