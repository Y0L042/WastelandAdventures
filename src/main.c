#include "main.h"

/* --- C Libraries --- */
#include <stdlib.h>
#include <stdio.h>


/* --- Libraries --- */
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <flecs.h>

/* --- Utilities --- */
#include "log.h"

/* --- Game Settings --- */
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const Color BG_COLOR = { 0, 0, 0, 255 };
const int TARGET_FPS = 60;
const char *GAME_TITLE = "Wasteland Adventures\0";

/* --- Extern Variables --- */
ecs_world_t *global_world;

/* --- Local Variables --- */
static int game_should_quit = 0;

/* --- Private Function Declarations --- */
static void initialize();
static void ready();
static void handle_input();
static void update(double delta);
static void physics_update(double delta);
static void draw(double delta);
static void handle_ui(double delta);
static void quit();

int main()
{
	double frame_time;

	initialize();
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE);
	SetTargetFPS(TARGET_FPS);       
	ready();
	while (!game_should_quit)    
	{
		frame_time = GetFrameTime();

		handle_input();
		update(frame_time);
		physics_update(frame_time);

		BeginDrawing();
			ClearBackground(BG_COLOR);

#ifdef CAMERA2D_H
            CameraComponent *cc = ecs_ref_get(gameplay_world, &cc_ref, CameraComponent);
            if (cc != NULL)
            {
                Camera2D c = cc->camera;
                BeginMode2D(c);
                    draw(frame_time);
                EndMode2D();
            }
#endif /* CAMERA2D_H */        

			handle_ui(frame_time);
			DrawFPS(10, 10);
		EndDrawing();

#ifdef INPUT_H
        consume_key_buffer();
#endif /* INPUT_H */
	}
	CloseWindow();
	quit();

    return 0;
}



static void initialize()
{

}

static void ready()
{

}

static void handle_input()
{

}

static void update(double delta)
{

}

static void physics_update(double delta)
{

}

static void draw(double delta)
{

}

static void handle_ui(double delta)
{

}

static void quit()
{

}
