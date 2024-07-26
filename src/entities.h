#ifndef ENTITIES_H
#define ENTITIES_H

#include <raylib.h>
#include "flecs.h"
#include "log.h"

#include "main.h"
#include "turnbasedsystem.h"
#include "components.h"
#include "glyph.h"

extern ecs_entity_t g_ent_player;
extern ecs_entity_t g_ent_camera;

void ent_player_create(
		ecs_entity_t *ent_player,
		ecs_world_t *world, 
		TurnManager *tm, 
		Grid* grid_worldspace,
		Tileset *tileset
	);




void ent_camera_create(
		ecs_entity_t *ent_camera,
		ecs_world_t *world,
		ecs_entity_t *entity_target
	);

#endif // ENTITIES_H
