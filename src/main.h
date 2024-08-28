#ifndef MAIN_H
#define MAIN_H

#include <raylib.h>
#include "flecs.h"

#include "turnbasedsystem.h"
#include "grid.h"
#include "glyph.h"

extern const int g_SCREEN_WIDTH;
extern const int g_SCREEN_HEIGHT;
extern const Color g_BG_COLOR;

extern ecs_world_t *g_world;

#endif // MAIN_H
