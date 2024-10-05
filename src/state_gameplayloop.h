#ifndef STATE_GAMEPLAYLOOP_H
#define STATE_GAMEPLAYLOOP_H

#include <flecs.h>
#include "statemachine.h"

typedef enum GameplayloopErrors {
    GAMEPLAYWORLD_CREATION_SUCCEEDED = 0,
    GAMEPLAYWORLD_CREATION_FAILED,
    GAMEPLAYWORLD_FREE_SUCCEEDED,
    GAMEPLAYWORLD_FREE_FAILED
} GameplayloopErrors;

/* Extern Variables */
extern ecs_world_t *gameplay_world;

/* Extern Functions */
void state_gameplayloop_register(SM_Machine *sm, SM_State *state);

#endif /* STATE_GAMEPLAYLOOP_H */
