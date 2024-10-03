#include "state_mainmenu.h"

#include "statemachine.h"
#include "states.h"

/* --- SM Function Prototypes --- */
static void enter_state(void);
static void update(double delta);
static void physics_update(double delta);
static void handle_ui(double delta);
static void draw(double delta);
static void exit_state(void);

void state_mainmenu_register()
{
    state_mainmenu.state_enter = enter_state;
    state_mainmenu.state_update = update;
    state_mainmenu.state_physics_update = physics_update;
    state_mainmenu.state_handle_ui = handle_ui;
    state_mainmenu.state_draw = draw;
    state_mainmenu.state_exit = exit_state;

    sm_register_state(&game_statemachine, &state_mainmenu, "STATE_MAINMENU");
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

}

static void handle_ui(double delta)
{

}

static void draw(double delta)
{

}

static void exit_state(void)
{

}
