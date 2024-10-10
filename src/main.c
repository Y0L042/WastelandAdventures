#include "main.h"

#define FLECS_JSON

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

/* --- Systems --- */
#include "states.h"
#define CAMERA2D
#include "camera.h"
#include "ecs_components.h"

/* --- Game Settings --- */
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const Color BG_COLOR = { 0, 0, 0, 255 };
const int TARGET_FPS = 60;
const char *GAME_TITLE = "Wasteland Adventures\0";

/* --- Extern Variables --- */
ecs_world_t *global_world;

/* Game States */
SM_Machine game_statemachine;
SM_State state_mainmenu;
SM_State state_pausemenu;
SM_State state_gameplayloop;
SM_State state_gameover_death;

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

#ifdef CAMERA2D
            if (is_active_camera2d_set()) {
                log_info("Active Camera2D is set.");
                Camera2DComponent *cc = ecs_ref_get(active_camera_world, 
                                                    active_camera2d_ref, Camera2DComponent);
                Camera2D c = cc->camera;
                BeginMode2D(c);
                    draw(frame_time);
                EndMode2D();
            } else {
                log_info("No Camera2D is set!");
                draw(frame_time);
            }
#endif /* CAMERA2D */        
#ifndef CAMERA2D
            draw(frame_time);
#endif /* NOT CAMERA2D */

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
    /* Only for stuff that NEEDS to run before window is initialized */
}

static void ready()
{
    /* Initialize AssetManager */
    assetmanager_initialize();
    assetmanager_load_texture("./assets/RDE_8x8.png");

    /* Register StateMachine and States */
	sm_create_state_machine(&game_statemachine, "GAME_STATEMACHINE");	
	state_mainmenu_register(&game_statemachine, &state_mainmenu);
    state_gameplayloop_register(&game_statemachine, &state_gameplayloop);
    sm_switch_state_pointer(&game_statemachine, &state_mainmenu);
}

static void handle_input()
{

}

static void update(double delta)
{
    sm_execute_state_update(&game_statemachine, delta);
}

static void physics_update(double delta)
{
    sm_execute_state_physics_update(&game_statemachine, delta);
}

static void draw(double delta)
{
    sm_execute_state_draw(&game_statemachine, delta);
}

static void handle_ui(double delta)
{
    sm_execute_state_handle_ui(&game_statemachine, delta);
}

static void quit()
{

}

void force_terminate_game()
{
    game_should_quit = 1;
}


