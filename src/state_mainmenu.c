#include "state_mainmenu.h"

#include "main.h"
#include "statemachine.h"

void _state_mainmenu_enter();
void _state_mainmenu_handle_input();
void _state_mainmenu_update(double delta);
void _state_mainmenu_physics_update(double delta);
void _state_mainmenu_draw(double delta);
void _state_mainmenu_exit();

void state_mainmenu_register()
{
	state_mainmenu.state_enter = _state_mainmenu_enter;
	state_mainmenu.state_update = _state_mainmenu_update;
	state_mainmenu.state_physics_update = _state_mainmenu_physics_update;
	state_mainmenu.state_draw = _state_mainmenu_draw;
	state_mainmenu.state_exit = _state_mainmenu_exit;
}

void _state_mainmenu_enter()
{

}

void _state_mainmenu_handle_input()
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

void _state_mainmenu_draw(double delta)
{
	DrawText("Main Menu", 0, 0, 32, RAYWHITE); 
}

void _state_mainmenu_exit()
{

}

