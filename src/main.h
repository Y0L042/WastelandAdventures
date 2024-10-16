#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#include <raylib.h>
#include "flecs.h"
#include "log.h"

#include "turnbasedsystem.h"
#include "grid.h"
#include "glyph.h"
#include "statemachine.h"

#define DBPRINT(...) log_info(__VA_ARGS__)

extern const int g_SCREEN_WIDTH;
extern const int g_SCREEN_HEIGHT;
extern const Color g_BG_COLOR;

extern ecs_world_t *global_world; // For timers and UI tweeners
extern ecs_world_t *gameplay_world;
extern ecs_entity_t ent_screen_center;

/* Game States */
extern SM_Machine game_fsm;
extern SM_State state_mainmenu;
extern SM_State state_pausemenu;
extern SM_State state_gameplayloop;
extern SM_State state_gameover_death;

void quit_game(void);
void create_ecs_gameplay_world(void);
void clear_ecs_gameplay_world(void);
void  restart_ecs_gameplay_world(void);

#endif // MAIN_H
