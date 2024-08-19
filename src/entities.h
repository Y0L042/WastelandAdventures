#ifndef ENTITIES_H
#define ENTITIES_H

#include <raylib.h>
#include "flecs.h"
#include "log.h"

#include "main.h"
#include "turnbasedsystem.h"
#include "components.h"
#include "glyph.h"
#include "pathfinding.h"

const int ENT_PLAYER_COLL_LAYER = 0b1;
const int ENT_PLAYER_COLL_MASK = 0b1;
const int ENT_WALL_PERM_COLL_LAYER = 0b1;
const int ENT_WALL_PERM_COLL_MASK = 0b0;

extern ecs_entity_t g_ent_player;
extern ecs_entity_t g_ent_camera;
extern ecs_entity_t g_ent_dog;

void ent_player_create(
		ecs_entity_t *ent_player,
		ecs_world_t *world, 
		TurnManager *tm, 
		Grid* grid,
		Tileset *tileset,
        int grid_x, int grid_y
	);

void ent_camera_create(
		ecs_entity_t *ent_camera,
		ecs_world_t *world,
		ecs_entity_t entity_target
	);

void ent_dog_create(
		ecs_entity_t *ent_dog,
		ecs_world_t *world, 
		TurnManager *tm, 
		Grid *grid,
		Tileset *tileset,
		ecs_entity_t entity_target
	);

void ent_wall_perm_create(
        ecs_entity_t *ent_wall_perm,
        ecs_world_t *world,
        Grid *grid,
        Tileset *tileset,
        int grid_x, int grid_y
    );

#endif // ENTITIES_H
