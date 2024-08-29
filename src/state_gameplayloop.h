#ifndef STATE_GAMEPLAYLOOP_H
#define STATE_GAMEPLAYLOOP_H

#include <stdio.h>
#include "flecs.h"

extern const int WORLDSPACE_SIZE_X;
extern const int WORLDSPACE_SIZE_Y;

extern const int TILE_SIZE_X;
extern const int TILE_SIZE_Y;

extern ecs_ref_t cc_ref;  

void state_gameplayloop_register();
void state_gameplayloop_reset(void);

#endif // STATE_GAMEPLAYLOOP_H
