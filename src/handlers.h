#ifndef HANDLERS_H
#define HANDLERS_H

#include <raylib.h>
#include <raymath.h>
#include "flecs.h"

#include "components.h"
#include "glyph.h"

#include <stdio.h>

extern ecs_entity_t glyph_draw_sys;
extern ecs_entity_t grid_move_sys;
extern ecs_entity_t player_input_sys;
extern ecs_entity_t camera_move_sys;

void create_handlers(ecs_world_t* world);

void handler_grid_move(ecs_iter_t *it);
void handler_glyph_draw(ecs_iter_t* it);
void handler_player_input(ecs_iter_t* it);
void handler_camera_move(ecs_iter_t *it);

#endif // HANDLERS_H
