#include "state_mainmenu.h"

#include <raylib.h>
#include <raygui.h>

#include "main.h"
#include "statemachine.h"

void _state_mainmenu_enter();
void _state_mainmenu_update(double delta);
void _state_mainmenu_physics_update(double delta);
void _state_mainmenu_handle_ui(double delta);
void _state_mainmenu_draw(double delta);
void _state_mainmenu_exit();

void state_mainmenu_register()
{
	state_mainmenu.state_enter = _state_mainmenu_enter;
	state_mainmenu.state_update = _state_mainmenu_update;
	state_mainmenu.state_physics_update = _state_mainmenu_physics_update;
	state_mainmenu.state_handle_ui = _state_mainmenu_handle_ui;	
	state_mainmenu.state_draw = _state_mainmenu_draw;
	state_mainmenu.state_exit = _state_mainmenu_exit;
}

void _state_mainmenu_enter()
{

}

void _state_mainmenu_update(double delta)
{

}

void _state_mainmenu_physics_update(double delta)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		sm_switch_state(&game_fsm, "STATE_GAMEPLAYLOOP");
	}
}

void _state_mainmenu_handle_ui(double delta)
{
	int mid_x = g_SCREEN_WIDTH / 2;
	int mid_y = g_SCREEN_HEIGHT / 2;

	DrawText("Main Menu", mid_x-80, mid_y-16  , 32, RAYWHITE); 
	if (GuiButton((Rectangle){ mid_x-60, mid_y+50, 120, 30 }, "#131#PLAY")) 
	{
		sm_switch_state(&game_fsm, "STATE_GAMEPLAYLOOP");
	}
	if (GuiButton((Rectangle){ mid_x-60, mid_y+50 + 40*1.5, 120, 30 }, "QUIT")) 
	{
		quit_game();
	}
}

void _state_mainmenu_draw(double delta)
{

}

void _state_mainmenu_exit()
{

}

