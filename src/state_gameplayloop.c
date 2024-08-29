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
#include "pathfinding.h"
#include "map_generator.h"
#include "main.h"


const int WORLDSPACE_SIZE_X = 25;
const int WORLDSPACE_SIZE_Y = 25;

const int TILE_SIZE_X = 25;
const int TILE_SIZE_Y = 25;

static int gameplayloop_initialized = 0;

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
	create_ecs_world();
	cc_ref = ecs_ref_init(g_world, g_ent_camera, CameraComponent);

	gameplayloop_initialized = 1;

	tileset_initialize(&tileset, "./assets/RDE_8x8.png", 8, 8, TILE_SIZE_X, TILE_SIZE_Y, RAYWHITE);
	tileset_initialize(
			&cool_tileset, 
			"./assets/urizen_onebit_tileset__v1d1.png", 
			13, 13, 
			TILE_SIZE_X, TILE_SIZE_Y, 
			RAYWHITE
		);

	turnmanager_initialize(&turnmanager, g_world);

	grid_initialize(
            &grid_worldspace, 
            g_world,
            WORLDSPACE_SIZE_X, 
            WORLDSPACE_SIZE_Y, 
            TILE_SIZE_X, 
            TILE_SIZE_Y
        );

    create_walls(g_world, &grid_worldspace, &cool_tileset);

	ent_player_create(
			&g_ent_player, 
			g_world, 
			&turnmanager, 
			&grid_worldspace, 
			&cool_tileset,
            spawn_x, spawn_y
		);

	CameraComponent *cc = ecs_get_mut(g_world, g_ent_camera, CameraComponent);
	cc->target_entity = g_ent_player;

	ent_dog_create(
			&g_ent_dog,
			g_world,
			&turnmanager,
			&grid_worldspace,
			&cool_tileset,
			g_ent_player,
			spawn_x_dog, spawn_y_dog
		);
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
//	log_debug("physics_update() - start");
    handler_player_input(g_world);
	handler_glyph_ghost_spawn(g_world);
	handler_grid_move(g_world);
	handler_pathfinding(g_world);
	handler_camera_move(g_world);
	handler_turncounter_increment(g_world);

//	log_debug("physics_update() - end");
}

void _state_gameplayloop_handle_ui(double delta)
{
}

void _state_gameplayloop_draw(double delta)
{
	grid_draw(&grid_worldspace);
	handler_glyph_fade(g_world, delta);
 	handler_glyph_draw(g_world);


	/*
	Color color;
	const TurnComponent *player_tc = ecs_get(g_world, g_ent_player, TurnComponent);
	TurnComponentData *player_tc_d = player_tc->tc_d;
	int active = (player_tc_d->current_turn_state == TURNSTATE_ACTIVE) ? 1 : 0;
	if (active == 0)
	{
		color = RED;
	}
	else
	{
		color = GREEN;
	}

	float h_tile_size = (TILE_SIZE_X / 2) + 1;
	const Position *p = ecs_get(g_world, g_ent_player, Position);
	DrawCircleLines(p->x + h_tile_size, p->y + h_tile_size, h_tile_size * 1.25f, color);
	*/
}

void _state_gameplayloop_exit()
{
}

void state_gameplayloop_reset(void)
{
	gameplayloop_initialized = 0;
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

