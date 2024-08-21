#include "main.h"

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>	
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

#include <stdio.h>

const int g_SCREEN_WIDTH = 1280;
const int g_SCREEN_HEIGHT = 720;
const Color g_BG_COLOR = BLACK;

const int WORLDSPACE_SIZE_X = 25;
const int WORLDSPACE_SIZE_Y = 25;

const int TILE_SIZE_X = 25;
const int TILE_SIZE_Y = 25;

int spawn_x, spawn_y;
int spawn_x_dog, spawn_y_dog;

Grid grid_worldspace;
Tileset tileset;
Glyph player_glyph;
ecs_world_t *g_world;
TurnManager turnmanager;

void initialize();
void ready();
void handle_input();
void update(double delta);
void physics_update(double delta);
void draw(double delta);
void quit();

void create_walls(ecs_world_t *world, Grid *grid, Tileset *tileset);

#ifdef _WIN32
	int main()
#else
	int main(int argc, char *argv[])
#endif
{
	SetTraceLogLevel(0);
	initialize();
	InitWindow(g_SCREEN_WIDTH, g_SCREEN_HEIGHT, "Wasteland Adventures");
	SetTargetFPS(60);       
	double frame_time;
	ready();
	ecs_ref_t cc_ref = ecs_ref_init(g_world, g_ent_camera, CameraComponent);
	while (!WindowShouldClose())    
	{
		frame_time = GetFrameTime();
		handle_input();
		update(frame_time);
		physics_update(frame_time);

		BeginDrawing();
			const CameraComponent *cc = ecs_ref_get(g_world, &cc_ref, CameraComponent);
			Camera2D c = cc->camera;
			BeginMode2D(c);
			ClearBackground(g_BG_COLOR);
			draw(frame_time);
			EndMode2D();
			DrawFPS(10, 10);
		EndDrawing();
	}
	CloseWindow();
	quit();

	return 0;
}




void initialize()
{	
}

void ready()
{
	log_debug("ready() - start");

	g_world = ecs_init();
	create_components(g_world);

	tileset_initialize(&tileset, "./assets/RDE_8x8.png", 8, 8, TILE_SIZE_X, TILE_SIZE_Y, RAYWHITE);

	turnmanager_initialize(&turnmanager, g_world);

	grid_initialize(
            &grid_worldspace, 
            g_world,
            WORLDSPACE_SIZE_X, 
            WORLDSPACE_SIZE_Y, 
            TILE_SIZE_X, 
            TILE_SIZE_Y
        );

    create_walls(g_world, &grid_worldspace, &tileset);

	ent_player_create(
			&g_ent_player, 
			g_world, 
			&turnmanager, 
			&grid_worldspace, 
			&tileset,
            spawn_x, spawn_y
		);

	ent_camera_create(
			&g_ent_camera, 
			g_world,
			g_ent_player
		);

	ent_dog_create(
			&g_ent_dog,
			g_world,
			&turnmanager,
			&grid_worldspace,
			&tileset,
			g_ent_player,
			spawn_x_dog, spawn_y_dog
		);


	log_debug("ready() - complete");
}

void handle_input()
{
}

void update(double delta)
{
}

void physics_update(double delta)
{



//	log_debug("physics_update() - start");
    handler_player_input(g_world);
	handler_grid_move(g_world);
	handler_camera_move(g_world);
	handler_pathfinding(g_world);
	handler_turncounter_increment(g_world);

//	log_debug("physics_update() - end");
}

void draw(double delta)
{
	grid_draw(&grid_worldspace);
 	handler_glyph_draw(g_world);



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
}

void quit()
{
	ecs_fini(g_world);
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

