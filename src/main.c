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

ecs_world_t *g_world;

void initialize();
void ready();
void handle_input();
void update(double delta);
void physics_update(double delta);
void draw(double delta);
void quit();


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
	g_world = ecs_init();
	create_components(g_world);
	ent_camera_create(
			&g_ent_camera, 
			g_world,
			-1 // No player exists yet.
		);
}

void handle_input()
{
}

void update(double delta)
{
}

void physics_update(double delta)
{

}

void draw(double delta)
{

}

void quit()
{

}



