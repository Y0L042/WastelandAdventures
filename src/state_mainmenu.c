#include "state_mainmenu.h"

/* --- Utilities --- */
#include <raylib.h>
#include <raygui.h>
#include "log.h"

/* --- Dependencies --- */
#include "statemachine.h"
#include "states.h"
#include "main.h"

/* --- SM Function Prototypes --- */
static void enter_state(void);
static void update(double delta);
static void physics_update(double delta);
static void handle_ui(double delta);
static void draw(double delta);
static void exit_state(void);

void state_mainmenu_register(SM_Machine *sm, SM_State *state)
{
    state->state_enter = enter_state;
    state->state_update = update;
    state->state_physics_update = physics_update;
    state->state_handle_ui = handle_ui;
    state->state_draw = draw;
    state->state_exit = exit_state;

    sm_register_state(sm, state, "STATE_MAINMENU");
}

/* --- SM Functions --- */
static void enter_state(void)
{
}

static void update(double delta)
{

}

static void physics_update(double delta)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		sm_switch_state(&game_statemachine, "STATE_GAMEPLAYLOOP");
	}
	
	if (IsKeyPressed(KEY_Q))
	{
        force_terminate_game();
	}
}

static void handle_ui(double delta)
{
	int mid_x = SCREEN_WIDTH / 2;
	int mid_y = SCREEN_HEIGHT / 2;

	DrawText("Main Menu", mid_x-80, mid_y-16  , 32, RAYWHITE); 
	if (GuiButton((Rectangle){ mid_x-60, mid_y+50, 120, 30 }, "#131#PLAY")) 
	{
		sm_switch_state(&game_statemachine, "STATE_GAMEPLAYLOOP");
	}
	if (GuiButton((Rectangle){ mid_x-60, mid_y+50 + 40*1.5, 120, 30 }, "QUIT")) 
	{
        force_terminate_game();
	}
}

static void draw(double delta)
{

}

static void exit_state(void)
{

}
