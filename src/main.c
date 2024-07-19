#include "main.h"

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>	

#include "flecs.h"

#include "components.h"
#include "handlers.h"

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
ecs_entity_t player;
ecs_entity_t camera;
TurnManager turnmanager;

void initialize();
void ready();
void handle_input();
void update(double delta);
void physics_update(double delta);
void draw(double delta);
void quit();

void create_player(ecs_world_t *world);
void create_camera(ecs_world_t *world);


int main()
{
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

		CameraComponent *cc = ecs_get(g_world, camera, CameraComponent);
		Camera2D c = cc->camera;
		BeginDrawing();
			BeginMode2D(c);
			ClearBackground(g_BG_COLOR);
			draw(frame_time);
			EndMode2D();
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
	grid_initialize(&grid_worldspace, WORLDSPACE_SIZE_X, WORLDSPACE_SIZE_Y, TILE_SIZE_X, TILE_SIZE_Y);
//	grid_initialize_tiles(&grid_worldspace, TILE_SIZE_X, TILE_SIZE_Y); 

	tileset_initialize(&tileset, "./assets/RDE_8x8.png", 8, 8, TILE_SIZE_X, TILE_SIZE_Y, RAYWHITE);

	turnmanager_initialize(&turnmanager);

	g_world = ecs_init();
	printf("g_world created.\n");

	create_components(g_world);
	printf("components created.\n");

	create_handlers(g_world);
	printf("handlers created.\n");

	create_player(g_world);
	create_camera(g_world);
}

void handle_input()
{
}

void update(double delta)
{
}

void physics_update(double delta)
{
	ecs_run(g_world, player_input_sys, delta, NULL);
	ecs_run(g_world, grid_move_sys, delta, NULL);
	ecs_run(g_world, camera_move_sys, delta, NULL);
	turnmanager_execute_turn_statemachine(&turnmanager, delta);
}

void draw(double delta)
{
	grid_draw(&grid_worldspace);
	ecs_run(g_world, glyph_draw_sys, delta, NULL);
}

void quit()
{
	grid_free(&grid_worldspace);
	ecs_fini(g_world);
}


void create_player(ecs_world_t *world)
{
	printf("Player creation starting.\n");

	player = ecs_new_id(world);

    ecs_set(world, player, Position, { .x = 0, .y = 0 });
    ecs_set(world, player, Velocity, { .x = 0, .y = 0 });
    ecs_set(world, player, GridPosition, { .x = 0, .y = 0 });
    ecs_set(world, player, GridVelocity, { .x = 0, .y = 0 });
    ecs_set(world, player, GridComponent, { .grid = &grid_worldspace, .tile = NULL });
    ecs_set(world, player, Glyph, { .source_tile_x = 0, .source_tile_y = 4, .tileset = &tileset }); 

	printf("Player creation finished.\n");

}

void create_camera(ecs_world_t *world)
{
	camera = ecs_new_id(world);
	
	Camera2D new_camera = { 
			(Vector2){ (g_SCREEN_WIDTH / 2), (g_SCREEN_HEIGHT / 2) },
			(Vector2){ 0.0f, 0.0f },
			0,
			1
		};
	ecs_set(world, camera, CameraComponent, { 
			.camera = new_camera,
			.target_entity = player,
			.previous_target_entity = 0	
		});
}

