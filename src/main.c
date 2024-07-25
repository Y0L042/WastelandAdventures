#include "main.h"

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>	
#include "log.h"

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
TurnManager turnmanager;
ecs_entity_t player;
ecs_entity_t camera;

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

		CameraComponent *cc = ecs_get_mut(g_world, camera, CameraComponent);
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

	grid_initialize(&grid_worldspace, WORLDSPACE_SIZE_X, WORLDSPACE_SIZE_Y, TILE_SIZE_X, TILE_SIZE_Y);
//	grid_initialize_tiles(&grid_worldspace, TILE_SIZE_X, TILE_SIZE_Y); 

	tileset_initialize(&tileset, "./assets/RDE_8x8.png", 8, 8, TILE_SIZE_X, TILE_SIZE_Y, RAYWHITE);

	g_world = ecs_init();
	create_components(g_world);
	turnmanager_initialize(&turnmanager, g_world);
	create_player(g_world);
	create_camera(g_world);

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
	//log_debug("physics_update() - start");

    handler_player_input(g_world);
	handler_grid_move(g_world);
	handler_camera_move(g_world);
	handler_turncounter_increment(g_world);

	//log_debug("physics_update() - end");
}

void draw(double delta)
{
	grid_draw(&grid_worldspace);
 	handler_glyph_draw(g_world);


	Color color;
	const TurnComponent *tc = ecs_get(g_world, player, TurnComponent);
	int active = (tc->current_turn_state == TURNSTATE_ACTIVE) ? 1 : 0;
	if (active == 0)
	{
		color = RED;
	}
	else
	{
		color = GREEN;
	}

	float h_tile_size = (TILE_SIZE_X / 2) + 1;
	const Position *p = ecs_get(g_world, player, Position);
	DrawCircleLines(p->x + h_tile_size, p->y + h_tile_size, h_tile_size * 1.25f, color);
}

void quit()
{
	grid_free(&grid_worldspace);
	ecs_fini(g_world);
}


void create_player(ecs_world_t *world)
{
	player = ecs_new_id(world);
    ecs_add(world, player, TAG_Player);
    ecs_set(world, player, Position, { .x = 0, .y = 0 });
    ecs_set(world, player, Velocity, { .x = 0, .y = 0 });
    ecs_set(world, player, GridPosition, { .x = 0, .y = 0 });
    ecs_set(world, player, GridVelocity, { .x = 0, .y = 0 });
    ecs_set(world, player, GridComponent, { 
			.grid = &grid_worldspace, 
			.tile = NULL 
		});
    ecs_set(world, player, Glyph, { 
			.source_tile_x = 0, 
			.source_tile_y = 4,
			.tileset = &tileset 
		}); 
    ecs_add(world, player, TurnComponent);  
	turnmanager_add_turncomponent(&turnmanager, player);
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
