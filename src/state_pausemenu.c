#include "state_pausemenu.h"

#include <raylib.h>
#include <raygui.h>

#include "main.h"
#include "state_gameplayloop.h"
#include "statemachine.h"

void _state_pausemenu_enter();
void _state_pausemenu_update(double delta);
void _state_pausemenu_physics_update(double delta);
void _state_pausemenu_handle_ui(double delta);
void _state_pausemenu_draw(double delta);
void _state_pausemenu_exit();

void state_pausemenu_register()
{
	state_pausemenu.state_enter = _state_pausemenu_enter;
	state_pausemenu.state_update = _state_pausemenu_update;
	state_pausemenu.state_physics_update = _state_pausemenu_physics_update;
	state_pausemenu.state_handle_ui = _state_pausemenu_handle_ui;	
	state_pausemenu.state_draw = _state_pausemenu_draw;
	state_pausemenu.state_exit = _state_pausemenu_exit;
}

void _state_pausemenu_enter()
{

}

void _state_pausemenu_update(double delta)
{

}

void _state_pausemenu_physics_update(double delta)
{

}

void _state_pausemenu_handle_ui(double delta)
{
	int mid_x = g_SCREEN_WIDTH / 2;
	int mid_y = g_SCREEN_HEIGHT / 2;

	DrawText("Pause Menu", mid_x-80, mid_y-16  , 24, RAYWHITE); 
	if (GuiButton((Rectangle){ mid_x-60, mid_y+50, 120, 30 }, "#131#CONTINUE")) 
	{
		sm_switch_previous_state(&game_fsm);
	}

	if (GuiButton((Rectangle){ mid_x-60, mid_y+50 + 40, 120, 30 }, "EXIT TO MAIN MENU")) 
	{
		state_gameplayloop_reset();
		clear_ecs_world();	
		sm_switch_state(&game_fsm, "STATE_MAINMENU");
	}

	if (GuiButton((Rectangle){ mid_x-60, mid_y+50 + 40*3, 120, 30 }, "QUIT")) 
	{
		quit_game();
	}
}

void _state_pausemenu_draw(double delta)
{

}

void _state_pausemenu_exit()
{

}


