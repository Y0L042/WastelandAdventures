#include "entities.h"

#include "maths.h"
#include "state_gameplayloop.h"
#include "ai.h"
#include "tween.h"
#include "timer.h"
#include "debugdraw.h"

ecs_entity_t g_ent_player;
ecs_entity_t g_ent_camera;
ecs_entity_t g_ent_dog;


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


const int ENT_PLAYER_COLL_LAYER = 	COLL_LAYER_PLAYER;
const int ENT_PLAYER_COLL_MASK = 	COLL_LAYER_WORLD
									| COLL_LAYER_ENEMIES;

const int ENT_DOG_COLL_LAYER = 	COLL_LAYER_FRIENDLIES;
const int ENT_DOG_COLL_MASK = 	COLL_LAYER_WORLD 
								| COLL_LAYER_PLAYER
								| COLL_LAYER_ENEMIES;

const int ENT_WALL_PERM_COLL_LAYER = 	COLL_LAYER_WORLD;
const int ENT_WALL_PERM_COLL_MASK = 	COLL_LAYER_EMPTY;

const int ENT_FLOORTRAP_BASIC_COLL_LAYER = 	COLL_LAYER_TRAPS;
const int ENT_FLOORTRAP_BASIC_COLL_MASK = 	COLL_LAYER_PLAYER 
											| COLL_LAYER_FRIENDLIES 
											| COLL_LAYER_ENEMIES;

/* S - smart, D - dumb */
const int ENT_ENEMY_GROUND_COLL_LAYER = 	COLL_LAYER_ENEMIES;
const int ENT_ENEMY_D_GROUND_COLL_MASK = 	COLL_LAYER_WORLD
											| COLL_LAYER_PLAYER
											| COLL_LAYER_ENEMIES
											| COLL_LAYER_FRIENDLIES;
const int ENT_ENEMY_S_GROUND_COLL_MASK = 	COLL_LAYER_WORLD
											| COLL_LAYER_PLAYER
											| COLL_LAYER_FRIENDLIES
											| COLL_LAYER_ENEMIES
											| COLL_LAYER_TRAPS;


void ent_camera_create(
		ecs_entity_t *ent_camera,
		ecs_world_t *world,
		ecs_entity_t entity_target
	)
{
	*ent_camera = ecs_new(world);
	ecs_add(world, *ent_camera, TAG_EntEnabled);
	
	Camera2D new_camera = { 
			(Vector2){ (g_SCREEN_WIDTH / 2), (g_SCREEN_HEIGHT / 2) },
			(Vector2){ 0.0f, 0.0f },
			0,
			1
		};
	ecs_set(world, *ent_camera, CameraComponent, { 
			.camera = new_camera,
			.target_entity = entity_target,
			.previous_target_entity = 0	
		});
}

void callback_player_ondeath(ecs_world_t *world, ecs_entity_t entity, int value)
{
	state_gameplayloop_queue_gameover();
}

static ecs_entity_t ent = 0;
void callback_player_reset_onhurt()
{
	log_info("Reset ouch");
	if ((gameplay_world != NULL)  && ecs_is_alive(gameplay_world, ent))
	{
		Glyph *g = ecs_get_mut(gameplay_world, ent, Glyph);
		g->color = RAYWHITE;
	}	
}

void callback_player_onhurt(ecs_world_t *world, ecs_entity_t entity, int value)
{
	log_info("OUCH!");
	ent = entity;

	Glyph *g = ecs_get_mut(world, entity, Glyph);
	g->color = RED;
	timer_oneshot(global_world, 0.15, callback_player_reset_onhurt);
}

void ent_player_create(
		ecs_entity_t *ent_player,
		ecs_world_t *world, 
		TurnManager *tm, 
		Grid* grid,
		Tileset *tileset,
        int grid_x, int grid_y
	)
{
//    log_debug("ent_player_create START");
	
    *ent_player = ecs_new(world);
	ecs_add(world, *ent_player, TAG_EntEnabled);

    ecs_add(world, *ent_player, TAG_Player);
    int world_x, world_y;
    grid_pos_to_world_pos(grid, grid_x, grid_y, &world_x, &world_y);
    ecs_set(world, *ent_player, Position, { .x = world_x, .y = world_y });
	grid_create_gridposition(
			grid_x, grid_y,
			grid, 
			*ent_player,
			ENT_PLAYER_COLL_LAYER,
			ENT_PLAYER_COLL_MASK
		);

    ecs_set(world, *ent_player, Glyph, { 
			.source_tile_x = 27,//0, 
			.source_tile_y = 0,//4,
			.tileset = tileset,
			.color = RAYWHITE
		}); 
	turnmanager_create_turncomponent(tm, *ent_player);
	turnmanager_set_component_alias(tm, *ent_player, "PLAYER\0");

	ecs_set(world, *ent_player, HealthComponent, { 
			.health = 100, 
			._initial_health = 100,
			.callback_ondeath = callback_player_ondeath,
			.callback_onhurt = callback_player_onhurt
		});
    
//    log_debug("ent_player_create END");
}

void ent_dog_create(
		ecs_entity_t *ent_dog,
		ecs_world_t *world, 
		TurnManager *tm, 
		Grid* grid,
		Tileset *tileset,
		ecs_entity_t entity_target,
		int grid_x, int grid_y
	)
{
	*ent_dog = ecs_new(world);
	ecs_add(world, *ent_dog, TAG_EntEnabled);
    int world_x, world_y;
    grid_pos_to_world_pos(grid, grid_x, grid_y, &world_x, &world_y);
	ecs_set(world, *ent_dog, Position, { .x = world_x, .y = world_y });
	ecs_set(world, *ent_dog, Glyph, {
			.source_tile_x = 10,//13,
			.source_tile_y = 20,//6,
			.tileset = tileset,
			.color = BROWN
		});
	grid_create_gridposition(
			grid_x, grid_y,
			grid, 
			*ent_dog,
			ENT_DOG_COLL_LAYER,
			ENT_DOG_COLL_MASK
		);
	turnmanager_create_turncomponent(tm, *ent_dog);
	turnmanager_set_component_alias(tm, *ent_dog, "PLR_DOG\0");
	//turnmanager_disable_tc(tm, *ent_dog);
	cmp_add_PathComponent(world, *ent_dog, grid);
    ecs_set(world, *ent_dog, NPCTarget, { .target = entity_target });
	ecs_set(world, *ent_dog, GhostWhenMoving, { .fade_time = 5 });
}

void ent_wall_perm_create(
        ecs_entity_t *ent_wall_perm,
        ecs_world_t *world,
        Grid *grid,
        Tileset *tileset,
        int grid_x, int grid_y
    )
{
   *ent_wall_perm = ecs_new(world);
   int world_x, world_y;
   grid_pos_to_world_pos(grid, grid_x, grid_y, &world_x, &world_y);
   ecs_set(world, *ent_wall_perm, Position, { .x = world_x, .y = world_y });
   grid_create_gridposition(
			grid_x, grid_y,
			grid, 
			*ent_wall_perm,
			ENT_WALL_PERM_COLL_LAYER,
			ENT_WALL_PERM_COLL_MASK
		);
   int rand_x = maths_randbetween_int(0, 1);
   ecs_set(world, *ent_wall_perm, Glyph, {
           .source_tile_x = 0 + rand_x,//11,
           .source_tile_y = 2,//13,
           .tileset = tileset,
		   .color = LIGHTGRAY
        });
}

void ent_glyph_ghost_create(
		ecs_world_t *world,
		Glyph *glyph,
		double time,
		int world_x, int world_y
	)
{
	ecs_entity_t ent_glyph_ghost = ecs_new(world);
	ecs_add(world, ent_glyph_ghost, TAG_EntEnabled);
	ecs_set(world, ent_glyph_ghost, Position, { .x = world_x, .y = world_y });
	ecs_set(world, ent_glyph_ghost, Glyph, {
		   .source_tile_x =  glyph->source_tile_x,//11,
		   .source_tile_y =  glyph->source_tile_y,//13,
		   .tileset = glyph->tileset,
		   .color = glyph->color
		});
	ecs_set(world, ent_glyph_ghost, GlyphFade, { 
			.initial_time = time, 
			.time_left = time 
		});
}

void callback_floor_trap_basic_damage(TriggerArea *ta, ecs_world_t *world, DRay *entities)
{
	for (int i = 0; i < entities->count; i++)
	{
		ecs_entity_t ent = dray_get_value(entities, i, ecs_entity_t);
		if (ecs_get(world, ent, HealthComponent))
		{
			printf("Entity %d takes damage!\n", ent);
			ecs_set(world, ent, HurtComponent, { .hurt = 15 });
		}
	}
}

void ent_floor_trap_basic_create(
		ecs_entity_t *ent_floortrap_basic,
		ecs_world_t *world,
		TurnManager *tm, 
		Grid *grid,
		Tileset *tileset,
		int grid_x, int grid_y
	)
{
    *ent_floortrap_basic = ecs_new(world);
	ecs_add(world, *ent_floortrap_basic, TAG_EntEnabled);
    int world_x, world_y;
    grid_pos_to_world_pos(grid, grid_x, grid_y, &world_x, &world_y);
    ecs_set(world, *ent_floortrap_basic, Position, { .x = world_x, .y = world_y });
	grid_create_gridposition(
			grid_x, grid_y,
			grid, 
			*ent_floortrap_basic,
			ENT_FLOORTRAP_BASIC_COLL_LAYER,
			ENT_FLOORTRAP_BASIC_COLL_MASK
		);

    ecs_set(world, *ent_floortrap_basic, Glyph, { 
			.source_tile_x = 3, 
			.source_tile_y = 2,
			.tileset = tileset,
			.color = { 255, 255, 255, 100 }
		}); 

	ecs_set(world, *ent_floortrap_basic, TriggerArea, {
			.rad = 0,
			.mode = 's',
			.area_mask = ENT_FLOORTRAP_BASIC_COLL_MASK,
			.callback = callback_floor_trap_basic_damage
		});

	turnmanager_create_turncomponent(tm, *ent_floortrap_basic);
	turnmanager_set_component_alias(tm, *ent_floortrap_basic, "FLRTRAP_B\0");
}

void ent_kobold_set_target(VisionArea *va, ecs_world_t *world, DRay *targets)
{
	ecs_entity_t owner = va->gridarea_ent;
	for (int i = 0; i < targets->count; i++)
	{
		ecs_entity_t ent = dray_get_value(targets, i, ecs_entity_t);
/* 		Only target entities with health */
		if (ecs_get(world, ent, HealthComponent))
		{
			ecs_set(world, va->gridarea_ent, NPCTarget, { .target = ent });

			const Position *target_p = ecs_get(world, ent, Position);
			if (target_p == NULL) { return; }
			const Position *owner_p = ecs_get(world, owner, Position);
			if (owner_p == NULL) { return; }

			debugdraw_add_line(owner_p->x+(float)TILE_SIZE_X/2,
					owner_p->y+(float)TILE_SIZE_Y/2,
					target_p->x+(float)TILE_SIZE_X/2,
					target_p->y+(float)TILE_SIZE_Y/2,
					GREEN, 2.5, DBG_DRAW);
		}
	}
}

int ent_kobold_ai(AIComponent *ai)
{
	log_info("KOBOLD AI");
	/*
	 * An entity can only take ONE action per turn. The AI's job is to choose
	 * what action must be taken. Must they attack, move, heal... They decide by
	 * adding the correct component to themselves, which will be executed next
	 * (the same turn).
	 */

	// Read targets in sight from blackboard. 
	// If enemy spotted,
	// 		if enemy close, attack enemy
	// 		else move closer to enemy
	// 	else
	// 		randomly
	// 			patrol
	// 			or wait
	// cmp_add_PathComponent(world, *ent_kobold, grid);
}

void ent_kobold_create(
		ecs_entity_t *ent_kobold,
		ecs_world_t *world,
		TurnManager *tm,
		Grid *grid,
		Tileset *tileset,
		int grid_x, int grid_y
	)
{
	*ent_kobold = ecs_new(world);
	ecs_add(world, *ent_kobold, TAG_EntEnabled);

    int world_x, world_y;
    grid_pos_to_world_pos(grid, grid_x, grid_y, &world_x, &world_y);
    ecs_set(world, *ent_kobold, Position, { .x = world_x, .y = world_y });
	grid_create_gridposition(
			grid_x, grid_y,
			grid, 
			*ent_kobold,
			ENT_ENEMY_GROUND_COLL_LAYER,
			ENT_ENEMY_D_GROUND_COLL_MASK
		);

    ecs_set(world, *ent_kobold, Glyph, { 
			.source_tile_x = 27, 
			.source_tile_y = 2,
			.tileset = tileset,
			.color = GREEN
		}); 
	ecs_set(world, *ent_kobold, GhostWhenMoving, { .fade_time = 5 });
	turnmanager_create_turncomponent(tm, *ent_kobold);
	turnmanager_set_component_alias(tm, *ent_kobold, "KOBOLD\0");
	// turnmanager_disable_tc(tm, *ent_kobold);

	ecs_set(world, *ent_kobold, VisionArea, {
			.gridarea_ent = *ent_kobold,
			.rad = 5,
			.mode = 'c',
			.area_mask = ENT_ENEMY_S_GROUND_COLL_MASK,
			.callback = ent_kobold_set_target
		});

	ecs_set(world, *ent_kobold, HealthComponent, { 
			.health = 100, 
			._initial_health = 100,
			.callback_ondeath = callback_player_ondeath,
			.callback_onhurt = callback_player_onhurt
		});

	ecs_set(world, *ent_kobold, AIComponent, {
			.world = world,
			.entity = *ent_kobold,
			.ai_main = ent_kobold_ai
		});
}

