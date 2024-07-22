#include "main.h"

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>	

#include "flecs.h"

#include "components.h"

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

    //create_queries(g_world);
	printf("queries created.\n");

	create_player(g_world);
	create_camera(g_world);

    TurnComponent *tc = ecs_get(g_world, player, TurnComponent);
    turncomponent_start_turn(tc);

}

void handle_input()
{
}

void update(double delta)
{
}

void physics_update(double delta)
{
    //handler_player_input(g_world);
}

void draw(double delta)
{
	grid_draw(&grid_worldspace);
    handler_draw_glyph(g_world);
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

    ecs_add(world, player, TAG_Player);
    ecs_set(world, player, Position, { .x = 0, .y = 0 });
    ecs_set(world, player, Velocity, { .x = 0, .y = 0 });
    ecs_set(world, player, GridPosition, { .x = 0, .y = 0 });
    ecs_set(world, player, GridVelocity, { .x = 0, .y = 0 });
    ecs_set(world, player, GridComponent, { .grid = &grid_worldspace, .tile = NULL });
    ecs_set(world, player, Glyph, { .source_tile_x = 0, .source_tile_y = 4, .tileset = &tileset }); 
    ecs_add(world, player, TurnComponent);  
    TurnComponent *tc = ecs_get(world, player, TurnComponent);
    turncomponent_initialize(tc, &turnmanager, player, world, 0);

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

