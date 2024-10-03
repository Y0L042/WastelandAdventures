#ifndef STATES_H
#define STATES_H

#include "statemachine.h"

/* --- State Headers (for registration) --- */
#include "state_mainmenu.h"

/* --- StateMachine --- */
extern SM_Machine game_statemachine;

/* --- States --- */
extern SM_State state_mainmenu;
extern SM_State state_pausemenu;
extern SM_State state_gameplayloop;
extern SM_State state_gameover_death;

#endif /* STATES_H */
