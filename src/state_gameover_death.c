#include "state_gameover_death.h"

#include <raylib.h>
#include <raygui.h>

#include "main.h"
#include "statemachine.h"
#include "state_gameplayloop.h"

void _state_gameover_death_initialize();
void _state_gameover_death_enter();
void _state_gameover_death_update(double delta);
void _state_gameover_death_physics_update(double delta);
void _state_gameover_death_handle_ui(double delta);
void _state_gameover_death_draw(double delta);
void _state_gameover_death_exit();

void state_gameover_death_register()
{
	state_gameover_death.state_enter = _state_gameover_death_enter;
	state_gameover_death.state_update = _state_gameover_death_update;
	state_gameover_death.state_physics_update = _state_gameover_death_physics_update;
	state_gameover_death.state_handle_ui = _state_gameover_death_handle_ui;
	state_gameover_death.state_draw = _state_gameover_death_draw;
	state_gameover_death.state_exit = _state_gameover_death_exit;
}

void state_gameover_death_reset(void)
{

}

void _state_gameover_death_initialize()
{

}

void _state_gameover_death_enter()
{
	state_gameplayloop_reset();
	clear_ecs_world();
}

void _state_gameover_death_update(double delta)
{

}

void _state_gameover_death_physics_update(double delta)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		sm_switch_state(&game_fsm, "STATE_GAMEPLAYLOOP");
	}
	if (IsKeyPressed(KEY_M))
	{
		sm_switch_state(&game_fsm, "STATE_MAINMENU");
	}
	if (IsKeyPressed(KEY_Q))
	{
		quit_game();
	}
}

void _state_gameover_death_handle_ui(double delta)
{
	int mid_x = g_SCREEN_WIDTH / 2;
	int mid_y = g_SCREEN_HEIGHT / 2;

	DrawText("Game Over!", mid_x-80, mid_y-16  , 32, RED); 
	if (GuiButton((Rectangle){ mid_x-60, mid_y+50, 120, 30 }, "#131#PLAY")) 
	{
		sm_switch_state(&game_fsm, "STATE_GAMEPLAYLOOP");
	}
	if (GuiButton((Rectangle){ mid_x-60, mid_y+100 + 40, 120, 30 }, "EXIT TO MAIN MENU")) 
	{
		sm_switch_state(&game_fsm, "STATE_MAINMENU");
	}
	if (GuiButton((Rectangle){ mid_x-60, mid_y+120 + 40*1.5, 120, 30 }, "QUIT")) 
	{
		quit_game();
	}
}

void _state_gameover_death_draw(double delta)
{

}

void _state_gameover_death_exit()
{

}

