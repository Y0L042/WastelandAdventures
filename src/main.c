#include "main.h"

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>	
#include "log.h"

#include <flecs.h>
#include "dray.h"
#include "maths.h"
#include "debugdraw.h"

#include "components.h"
#include "entities.h"
#include "input.h"

#include "state_mainmenu.h"
#include "state_pausemenu.h"
#include "state_gameplayloop.h"
#include "state_gameover_death.h"

#include "grid.h"
#include "glyph.h"
#include "pathfinding.h"
#include "map_generator.h"
#include "tween.h"
#include "timer.h"


ecs_world_t *global_world;

const int g_SCREEN_WIDTH = 1280;
const int g_SCREEN_HEIGHT = 720;
const Color g_BG_COLOR = { 0, 0, 0, 255 };

/* Game States */
SM_Machine game_fsm;
SM_State state_mainmenu;
SM_State state_pausemenu;
SM_State state_gameplayloop;
SM_State state_gameover_death;

static int is_ecs_gameplay_world_created = 0;
ecs_world_t *gameplay_world;
ecs_entity_t ent_screen_center;

int game_should_quit = 0;

void initialize();
void ready();
void handle_input();
void update(double delta);
void physics_update(double delta);
void draw(double delta);
void handle_ui(double delta);
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
	while (!game_should_quit)    
	{
		frame_time = GetFrameTime();

		handle_input();
		update(frame_time);
		physics_update(frame_time);

		BeginDrawing();
			ClearBackground(g_BG_COLOR);

			if (is_ecs_gameplay_world_created)
			{
				const CameraComponent *cc = ecs_ref_get(gameplay_world, &cc_ref, CameraComponent);
				if (cc != NULL)
				{
					Camera2D c = cc->camera;
					BeginMode2D(c);
						draw(frame_time);
					EndMode2D();
				}
			}

			handle_ui(frame_time);
			DrawFPS(10, 10);
		EndDrawing();

        consume_key_buffer();
	}
	CloseWindow();
	quit();

	return 0;
}




void initialize()
{	
	global_world = ecs_init();
	create_components(global_world);

	sm_create_state_machine(&game_fsm, "MAIN_FSM");	

	sm_register_state(&game_fsm, &state_mainmenu, "STATE_MAINMENU");
	state_mainmenu_register();

	sm_register_state(&game_fsm, &state_pausemenu, "STATE_PAUSEMENU");
	state_pausemenu_register();

	sm_register_state(&game_fsm, &state_gameplayloop, "STATE_GAMEPLAYLOOP");
	state_gameplayloop_register();

	sm_register_state(&game_fsm, &state_gameover_death, "STATE_GAMEOVER_DEATH");
	state_gameover_death_register();
}

void ready()
{
	sm_switch_state_pointer(&game_fsm, &state_mainmenu);
}

void handle_input()
{
	int _quit_game = 0;

#ifdef DEBUG 
	_quit_game = IsKeyPressed(KEY_F4);
#endif
#ifdef RELEASE 
	_quit_game = IsKeyPressed(KEY_LEFT_ALT) && IsKeyPressed(KEY_F4);
#endif
	if (_quit_game)
	{
		quit_game();
	}

    process_key_buffer();
}

void update(double delta)
{
	sm_execute_state_update(&game_fsm, delta);
}

void physics_update(double delta)
{
	sm_execute_state_physics_update(&game_fsm, delta);
	handler_process_timers(global_world, delta);
}

void handle_ui(double delta)
{
	sm_execute_state_handle_ui(&game_fsm, delta);
	debugdraw_handle_ui(delta);
};

void draw(double delta)
{
	sm_execute_state_draw(&game_fsm, delta);
	debugdraw_draw(delta);
}

void quit()
{
}

void quit_game(void)
{
	game_should_quit = 1;
}

void create_ecs_gameplay_world(void)
{
	if (is_ecs_gameplay_world_created) { return; }
	is_ecs_gameplay_world_created = 1;
	gameplay_world = ecs_init();
	create_components(gameplay_world);

	ent_screen_center = ecs_new(gameplay_world);
	ecs_set(gameplay_world, ent_screen_center, Position, { .x = g_SCREEN_WIDTH/2, .y = g_SCREEN_HEIGHT/2 });

	ent_camera_create(
			&g_ent_camera, 
			gameplay_world,
			ent_screen_center
		);
}

void clear_ecs_gameplay_world(void)
{
	free_queries(gameplay_world);
	ecs_fini(gameplay_world);
	is_ecs_gameplay_world_created = 0;
	gameplay_world = NULL;
}

void restart_ecs_gameplay_world(void)
{
	clear_ecs_gameplay_world();
	create_ecs_gameplay_world();
}

