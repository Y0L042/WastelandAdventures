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

#ifndef COLL_LAYERS_DEFINED
#define COLL_LAYERS_DEFINED
	#define COLL_LAYER_EMPTY 		0b00000000
	#define COLL_LAYER_ALL			0b11111111
	#define COLL_LAYER_WORLD 		0b00000001
	#define COLL_LAYER_PLAYER 		0b00000010
	#define COLL_LAYER_FRIENDLIES 	0b00000100
	#define COLL_LAYER_ENEMIES 		0b00001000
	#define COLL_LAYER_TRAPS 		0b00010000
	#define COLL_LAYER_1			0b00100000
	#define COLL_LAYER_2			0b01000000
	#define COLL_LAYER_3			0b10000000
#endif // COLL_LAYERS_DEFINED

extern const int ENT_PLAYER_COLL_LAYER;
extern const int ENT_PLAYER_COLL_MASK;
extern const int ENT_DOG_COLL_LAYER;
extern const int ENT_DOG_COLL_MASK;
extern const int ENT_WALL_PERM_COLL_LAYER;
extern const int ENT_WALL_PERM_COLL_MASK;
extern const int ENT_FLOORTRAP_BASIC_COLL_LAYER;
extern const int ENT_FLOORTRAP_BASIC_COLL_MASK;

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
		Grid* grid,
		Tileset *tileset,
		ecs_entity_t entity_target,
		int grid_x, int grid_y
	);

void ent_wall_perm_create(
        ecs_entity_t *ent_wall_perm,
        ecs_world_t *world,
        Grid *grid,
        Tileset *tileset,
        int grid_x, int grid_y
    );

void ent_glyph_ghost_create(
		ecs_world_t *world,
		Glyph *glyph,
		double time,
		int world_x, int world_y
	);

void callback_floor_trap_basic_damage(ecs_world_t *world, DRay *entities);

void ent_floor_trap_basic_create(
		ecs_entity_t *ent_floortrap_basic,
		ecs_world_t *world,
		TurnManager *tm, 
		Grid *grid,
		Tileset *tileset,
		int grid_x, int grid_y
	);
#endif // ENTITIES_H
