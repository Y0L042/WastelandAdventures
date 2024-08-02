#include "main.h"

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>	
#include "log.h"

#include "flecs.h"

#include "components.h"
#include "entities.h"

#include "grid.h"
#include "glyph.h"

#include <stdio.h>

const int g_SCREEN_WIDTH = 1280;
const int g_SCREEN_HEIGHT = 720;
const Color g_BG_COLOR = BLACK;

const int WORLDSPACE_SIZE_X = 100;
const int WORLDSPACE_SIZE_Y = 100;

const int TILE_SIZE_X = 25;
const int TILE_SIZE_Y = 25;

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

void create_walls();

int main()
{
	SetTraceLogLevel(0);
	initialize();
	InitWindow(g_SCREEN_WIDTH, g_SCREEN_HEIGHT, "Wasteland Adventures");
	SetTargetFPS(60);             
	ready();
	while (!WindowShouldClose())    
	{
		double frame_time = GetFrameTime();
		handle_input();
		update(frame_time);
		physics_update(frame_time);

		CameraComponent *cc = ecs_get_mut(g_world, g_ent_camera, CameraComponent);
		Camera2D c = cc->camera;
		BeginDrawing();
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

	ent_player_create(
			&g_ent_player, 
			g_world, 
			&turnmanager, 
			&grid_worldspace, 
			&tileset
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
			g_ent_player
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
	grid_free(&grid_worldspace);
	ecs_fini(g_world);
}


void create_walls()
{

}
