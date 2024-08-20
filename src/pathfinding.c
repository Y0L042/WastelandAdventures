#include "pathfinding.h"
#include "components.h"
#include <raylib.h>

DRay* pathfind_astar(Grid *grid, int from_x, int from_y, int to_x, int to_y, int coll_mask);
DRay* find_path(Grid *grid, int from_x, int from_y, int to_x, int to_y, int coll_mask)
{
	/*
	DRay *path = (DRay *)malloc(sizeof(DRay));
	dray_init_values(path, Vector2);
	Vector2 next_pos = { to_x - 1, to_y - 1 };
	dray_add_value(path, next_pos, Vector2);

	return path;
	*/

	return pathfind_astar(grid, from_x, from_y, to_x, to_y, coll_mask);
}

enum ASTAR_NodeState { ASTAR_NONE, ASTAR_OPENLIST, ASTAR_CLOSEDLIST, ASTAR_IGNORE };
#define ASTARNODE_EMPTY { 0, 0, 0, 0, 0, 0, 0, NULL, ASTAR_NONE }
typedef struct AStarNode {
	int x;
	int y;
	int t_x;
	int t_y;
	int f;
	int g;
	int h;
	int coll_layer;
	struct AStarNode *parent;
	enum ASTAR_NodeState state;
	Grid *grid;
} AStarNode;

static DRay *open_list = NULL;
static DRay *closed_list = NULL;

void _astarnode_init(AStarNode *node, int x, int y, int t_x, int t_y, int coll_layer, AStarNode *parent, Grid *grid);
DRay* _pathfind_get_neighbors(AStarNode ***nodemap, AStarNode *node, int coll_mask);

DRay* pathfind_astar(Grid *grid, int from_x, int from_y, int to_x, int to_y, int coll_mask)
{
	/* Create grid for pathfinding */
	AStarNode **nodemap = GRID_CREATE_2D(grid->width, grid->height, AStarNode);
	for (int _x = 0; _x < grid->width; _x++)
	{
		for (int _y = 0; _y < grid->height; _y++)
		{
			_astarnode_init(&nodemap[_x][_y], _x, _y, to_x, to_y, grid->arr_coll_layers[_x][_y], NULL, grid);
		}
	}

	/* Create open/closed lists */
	open_list = (DRay *)malloc(sizeof(DRay));
	dray_init_pointers(open_list, AStarNode);
	closed_list = (DRay *)malloc(sizeof(DRay));
	dray_init_pointers(closed_list, AStarNode);

	int path_found = 0;
	AStarNode *target_node;

	AStarNode *start_node = &nodemap[from_x][from_y];
	dray_add_pointer(open_list, start_node);
	start_node->state = ASTAR_OPENLIST;

	while (open_list->count != 0)
	{
		AStarNode *current_node = dray_get_pointer(open_list, 0, AStarNode);
		target_node = current_node;
		int current_node_idx = 0;
		for (int i = 0; i < open_list->count; i++)
		{
			AStarNode *node_i = dray_get_pointer(open_list, i, AStarNode);
			if (node_i->f < current_node->f)
			{
				current_node = node_i;
				current_node_idx = i;
			}
		}

		if ((current_node->x == current_node->t_x) && (current_node->y == current_node->t_y)) 
		{
			path_found = 1; 
			break; 
		}

		dray_add_pointer(closed_list, current_node);
		current_node->state = ASTAR_CLOSEDLIST;
		dray_remove_idx(open_list, current_node_idx);

		DRay *neighbors = _pathfind_get_neighbors(&nodemap, current_node, coll_mask);
		for (int i = 0; i < neighbors->count; i++)
		{
			AStarNode *neighbor = dray_get_pointer(neighbors, i, AStarNode);
			if (neighbor->state == ASTAR_NONE) /* If neighbor is not on openlist, calculate values and add */
			{
				neighbor->parent = current_node;
				neighbor->g = current_node->g + 1;
				neighbor->h = Vector2DistanceSqr((Vector2){ neighbor->t_x, neighbor->t_y }, (Vector2){ neighbor->x, neighbor->y });
				neighbor->f = neighbor->g + neighbor->h;

				dray_add_pointer(open_list, neighbor);
				neighbor->state = ASTAR_OPENLIST;
			}
			else
			{
				if (neighbor->g > (current_node->g + 1)) /* If path from node to neighbor is shorter than neighbor's pref path, update */
				{
					neighbor->parent = current_node;
					neighbor->g = current_node->g + 1;
					neighbor->f = neighbor->g + neighbor->h;
				}
			}
		}
	}
	
	if (path_found)
	{
		DRay *path = (DRay *)malloc(sizeof(DRay));
		dray_init_values(path, Vector2);
		AStarNode *iter_node = target_node;
		printf("%d %d\n", iter_node->x, iter_node->y);
		Vector2 pos = { (float)iter_node->x, (float)iter_node->y };
		printf("%.f %.f\n", pos.x, pos.y);
		//dray_add_value(path, pos, Vector2);
		do {
			pos.x =  (float)iter_node->x;
			pos.y = (float)iter_node->y;
			log_info("PATHFIND PATH POS: { %.f, %.f }", pos.x, pos.y);
			DrawCircle(
					pos.x * grid->tile_width, 
					pos.y * grid->tile_height, 
					20, 
					YELLOW
				);
			dray_add_value(path, pos, Vector2);
			iter_node = iter_node->parent;	
		} while (iter_node->parent != NULL);

		log_info("TARGET_AQUIRED");
		return path;
	}
	else
	{
		log_info("TARGET_NOT_FOUND");
		return NULL;
	}

	return NULL;
}

void _astarnode_init(AStarNode *node, int x, int y, int t_x, int t_y, int coll_layer, AStarNode *parent, Grid *grid)
{
	node->x = x;
	node->y = y;
	node->t_x = t_x;
	node->t_y = t_y;
	node->parent = parent;
	node->coll_layer = coll_layer;
	node->g = node->parent == NULL ? 0 : node->parent->g + 1;
	node->h = Vector2DistanceSqr((Vector2){ t_x, t_y }, (Vector2){ x, y });
	node->f = node->g + node->h;
	node->grid = grid;
}

DRay* _pathfind_get_neighbors(AStarNode ***nodemap, AStarNode *node, int coll_mask)
{
	DRay *neighbors = (DRay *)malloc(sizeof(DRay));
	dray_init_pointers(neighbors, AStarNode);

	int x = node->x;
	int y = node->y;
	Grid *grid = node->grid;
	AStarNode **map = *nodemap;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if ((i == 0) && (j == 0)) { continue; }
			int nx = x + i;
			int ny = y + j;
			if ((nx < 0) || (nx >= grid->width)) { continue; }
			if ((ny < 0) || (ny >= grid->height)) { continue; }

			AStarNode *neighbor = &map[nx][ny];			/* Include target pos, even if colliding */
			if (
					(neighbor->coll_layer & coll_mask) 
					&& ((nx != neighbor->t_x) || (ny != neighbor->t_y))
				) 
			{ 
				continue; 
			} /* If collision found */
			if (neighbor->state == ASTAR_CLOSEDLIST) { continue; }

			dray_add_pointer(neighbors, neighbor); 
		}
	}

	return neighbors;
}




