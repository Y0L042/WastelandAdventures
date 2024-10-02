#define STATE_GAMEPLAYLOOP _state_gameplayloop_

#include "state_gameplayloop.h"

#include <raylib.h>
#include "log.h"
#include "flecs.h"
#include "dray.h"
#include "maths.h"
#include "components.h"
#include "entities.h"

#include "grid.h"
#include "glyph.h"
#include "map_generator.h"
#include "main.h"
#include "timer.h"
#include "tween.h"


const int WORLDSPACE_SIZE_X = 25;
const int WORLDSPACE_SIZE_Y = 25;

const int TILE_SIZE_X = 25;
const int TILE_SIZE_Y = 25;

static int gameplayloop_initialized = 0;
static int gameover_queued = 0;

int spawn_x, spawn_y;
int spawn_x_dog, spawn_y_dog;
ecs_ref_t cc_ref;  
Grid grid_worldspace;
Tileset tileset;
Tileset cool_tileset;
TurnManager turnmanager;

void _state_gameplayloop_initialize();
void _state_gameplayloop_enter();
void _state_gameplayloop_update(double delta);
void _state_gameplayloop_physics_update(double delta);
void _state_gameplayloop_handle_ui(double delta);
void _state_gameplayloop_draw(double delta);
void _state_gameplayloop_exit();

void create_walls(ecs_world_t *world, Grid *grid, Tileset *tileset);
void create_traps(ecs_world_t *world, TurnManager *tm, Grid *grid, Tileset *tileset);
void create_enemies(ecs_world_t *world, TurnManager *tm, Grid *grid, Tileset *tileset);

void state_gameplayloop_register()
{
	state_gameplayloop.state_enter = _state_gameplayloop_enter;
	state_gameplayloop.state_update = _state_gameplayloop_update;
	state_gameplayloop.state_physics_update = _state_gameplayloop_physics_update;
	state_gameplayloop.state_handle_ui = _state_gameplayloop_handle_ui;
	state_gameplayloop.state_draw = _state_gameplayloop_draw;
	state_gameplayloop.state_exit = _state_gameplayloop_exit;
}

void _state_gameplayloop_initialize()
{
	gameplayloop_initialized = 1;

	create_ecs_gameplay_world();
	create_queries(gameplay_world);
	cc_ref = ecs_ref_init(gameplay_world, g_ent_camera, CameraComponent);


	tileset_initialize(&tileset, "./assets/RDE_8x8.png", 8, 8, TILE_SIZE_X, TILE_SIZE_Y, RAYWHITE);
	tileset_initialize(
			&cool_tileset, 
			"./assets/urizen_onebit_tileset__v1d1.png", 
			13, 13, 
			TILE_SIZE_X, TILE_SIZE_Y, 
			RAYWHITE
		);

	turnmanager_initialize(&turnmanager, gameplay_world);

	grid_initialize(
            &grid_worldspace, 
            gameplay_world,
            WORLDSPACE_SIZE_X, 
            WORLDSPACE_SIZE_Y, 
            TILE_SIZE_X, 
            TILE_SIZE_Y
        );

    create_walls(gameplay_world, &grid_worldspace, &cool_tileset);
	

	ent_player_create(
			&g_ent_player, 
			gameplay_world, 
			&turnmanager, 
			&grid_worldspace, 
			&cool_tileset,
            spawn_x, spawn_y
		);

	create_traps(gameplay_world, &turnmanager, &grid_worldspace, &cool_tileset);
	create_enemies(gameplay_world, &turnmanager, &grid_worldspace, &cool_tileset);

	CameraComponent *cc = ecs_get_mut(gameplay_world, g_ent_camera, CameraComponent);
	cc->target_entity = g_ent_player;

	ent_dog_create(
			&g_ent_dog,
			gameplay_world,
			&turnmanager,
			&grid_worldspace,
			&cool_tileset,
			g_ent_player,
			spawn_x_dog, spawn_y_dog
		);

    handler_init_gridposition(gameplay_world);
}

void _state_gameplayloop_enter()
{
	log_debug("ready() - start");

	if (!gameplayloop_initialized) { _state_gameplayloop_initialize(); }
	
	log_debug("ready() - complete");
}


void _state_gameplayloop_update(double delta)
{
	if (IsKeyPressed(KEY_ESCAPE))
	{
		/* TODO add state stack for pause/unpause */
		sm_switch_state_pointer(&game_fsm, &state_pausemenu);
	}
}

void _state_gameplayloop_physics_update(double delta)
{
	/* Pre-Action Systems */
	handler_glyph_ghost_spawn(gameplay_world);

	/* Input Gathering */
    handler_player_input(gameplay_world);
	handler_process_triggerareas(gameplay_world);
	handler_process_visionareas(gameplay_world);

	/* Decision Making */
	handler_process_ai(gameplay_world);

	/* Processing */
	handler_pathfinding(gameplay_world);
	handler_grid_move(gameplay_world);
	handler_process_hurt(gameplay_world);
	handler_process_death(gameplay_world);

	/* Post-Action Systems */
	handler_camera_move(gameplay_world);
	handler_turncounter_increment(gameplay_world);
	handler_tween_add_property(gameplay_world, delta);

	/* GameplayLoop Exit */
	if (gameover_queued) { state_gameplayloop_goto_gameover_death(); }
}

void _state_gameplayloop_handle_ui(double delta)
{
	handler_draw_health_ui(gameplay_world);
}

void _state_gameplayloop_draw(double delta)
{
	grid_draw(&grid_worldspace);
	handler_glyph_fade(gameplay_world, delta);
 	handler_glyph_draw(gameplay_world);
}

void _state_gameplayloop_exit()
{
}

void state_gameplayloop_reset(void)
{
	gameplayloop_initialized = 0;
	gameover_queued = 0;
}

void state_gameplayloop_goto_gameover_death()
{
	state_gameplayloop_reset();	
	sm_switch_state(&game_fsm, "STATE_GAMEOVER_DEATH");
}

void state_gameplayloop_queue_gameover()
{
	gameover_queued = 1;
}

void create_walls(ecs_world_t *world, Grid *grid, Tileset *tileset)
{

	int area = grid->width * grid->height;
	for (int i = 0; i < grid->width; i++)
	{
		for (int j = 0; j < grid->height; j++)
		{
			ecs_entity_t wall;
			ent_wall_perm_create(
					&wall,
					world,
					grid,
					tileset,
					i, j
				);
		}
	}

	DRay *walkable_tiles = mapgen_generate_RANDOMWALKER(grid);
	for (int i = 0; i < walkable_tiles->count; i++)
	{
		Vector2 pos = dray_get_value(walkable_tiles, i, Vector2);
		DRay *entities =  grid_get_entities_at(grid, pos.x, pos.y);
		for (int ent_idx = 0; ent_idx < entities->count; ent_idx++)
		{
			ecs_entity_t ent = dray_get_value(entities, ent_idx, ecs_entity_t);
			GridPosition *gp = ecs_get_mut(world, ent, GridPosition);
			gridposition_delete(gp);
			ecs_remove(world, ent, GridPosition);
			ecs_delete(world, ent);
			dray_clear_idx(entities, ent_idx);
		}
	}

	int rand_idx = maths_randbetween_int(0, walkable_tiles->count);
	Vector2 spawnpos = dray_get_value(walkable_tiles, rand_idx, Vector2);
	spawn_x = spawnpos.x;
	spawn_y = spawnpos.y;

	rand_idx = maths_randbetween_int(0, walkable_tiles->count);
	spawnpos = dray_get_value(walkable_tiles, rand_idx, Vector2);
	spawn_x_dog = spawnpos.x;
	spawn_y_dog = spawnpos.y;

	free(walkable_tiles);
}

void create_traps(ecs_world_t *world, TurnManager *tm, Grid *grid, Tileset *tileset)
{
	DRay walkable_tiles;
	dray_init_values(&walkable_tiles, Vector2);
	for (int _x = 0; _x < grid->width-1; _x++)
	{
		for (int _y = 0; _y < grid->height-1; _y++)
		{
			int grid_coll = grid->arr_coll_layers[_x][_y];
			if (
					((grid_coll & COLL_LAYER_WORLD) == 0)
					&& ((grid_coll & COLL_LAYER_TRAPS) == 0)
					&& ((grid_coll & COLL_LAYER_PLAYER) == 0)
					&& ((grid_coll & COLL_LAYER_FRIENDLIES) == 0)
					&& ((grid_coll & COLL_LAYER_ENEMIES) == 0)
				)
			{
				Vector2 _p = { _x, _y };
				dray_add_value(&walkable_tiles, _p, Vector2);
			}
		}
	}
	
	
	int spawn_count = maths_randbetween_int(4, 8);
	for (int _i = 0; _i < spawn_count; _i++)
	{
		int rand_idx = maths_randbetween_int(0, walkable_tiles.count - 1);
		Vector2 spawnpos = dray_get_value(&walkable_tiles, rand_idx, Vector2);
		ecs_entity_t trap;
		ent_floor_trap_basic_create(
				&trap,
				world,
				tm,
				grid,
				tileset,
				spawnpos.x, spawnpos.y
				);
		dray_remove_idx(&walkable_tiles, rand_idx);
	}
	dray_free(&walkable_tiles);
}

void create_enemies(ecs_world_t *world, TurnManager *tm, Grid *grid, Tileset *tileset)
{
	DRay walkable_tiles;
	dray_init_values(&walkable_tiles, Vector2);
	for (int _x = 0; _x < grid->width-1; _x++)
	{
		for (int _y = 0; _y < grid->height-1; _y++)
		{
			int grid_coll = grid->arr_coll_layers[_x][_y];
			if (
					((grid_coll & COLL_LAYER_WORLD) == 0)
					&& ((grid_coll & COLL_LAYER_TRAPS) == 0)
					&& ((grid_coll & COLL_LAYER_PLAYER) == 0)
					&& ((grid_coll & COLL_LAYER_FRIENDLIES) == 0)
					&& ((grid_coll & COLL_LAYER_ENEMIES) == 0)
				)
			{
				Vector2 _p = { _x, _y };
				dray_add_value(&walkable_tiles, _p, Vector2);
			}
		}
	}
	
	
	int spawn_count = maths_randbetween_int(4, 8);
	for (int _i = 0; _i < spawn_count; _i++)
	{
		int rand_idx = maths_randbetween_int(0, walkable_tiles.count - 1);
		Vector2 spawnpos = dray_get_value(&walkable_tiles, rand_idx, Vector2);
		ecs_entity_t enemy;
		ent_kobold_create(
				&enemy,
				world,
				tm,
				grid,
				tileset,
				spawnpos.x, spawnpos.y
				);
		dray_remove_idx(&walkable_tiles, rand_idx);
	}
	dray_free(&walkable_tiles);
}
