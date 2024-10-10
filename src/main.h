#ifndef MAIN_H
#define MAIN_H

/* --- Header Includes --- */
#include <flecs.h>
#include "assetmanager.h"

/* --- Extern Constants --- */
extern const int SCREEN_WIDTH, SCREEN_HEIGHT;

/* --- Extern Variables --- */
extern ecs_world_t *global_world;

/* --- Global Functions --- */
/* Quits to desktop */
void force_terminate_game();


#endif /* MAIN_H */
