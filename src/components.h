#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <raylib.h>
#include <raymath.h>
#include "log.h"
#include "flecs.h"
#include "grid.h"
#include "glyph.h"
#include "turnbasedsystem.h"

typedef Vector2 Position;
extern ECS_COMPONENT_DECLARE(Position);

typedef Vector2 Velocity;
extern ECS_COMPONENT_DECLARE(Velocity);

typedef Vector2 GridPosition;
extern ECS_COMPONENT_DECLARE(GridPosition);

typedef Vector2 GridVelocity;
extern ECS_COMPONENT_DECLARE(GridVelocity);

extern ECS_COMPONENT_DECLARE(GridComponent);

extern ECS_COMPONENT_DECLARE(Glyph);

typedef struct CameraComponent{
	Camera2D camera;
	ecs_entity_t target_entity;
	ecs_entity_t previous_target_entity;
} CameraComponent;
extern ECS_COMPONENT_DECLARE(CameraComponent);

extern ECS_COMPONENT_DECLARE(TurnComponent);
typedef struct TurnCountComponent TurnCountComponent; // Forward Declaration
extern ECS_COMPONENT_DECLARE(TurnCountComponent);

struct TAG_Player{ int empty; } ;
extern ECS_ENTITY_DECLARE(TAG_Player);

struct TAG_TurnActive{ int empty; } ;
extern ECS_ENTITY_DECLARE(TAG_TurnActive);

struct TAG_TurnIdle{ int empty; } ;
extern ECS_ENTITY_DECLARE(TAG_TurnIdle);


void create_components(ecs_world_t *world);




// Handlers

void create_queries(ecs_world_t *world);

void handler_glyph_draw(ecs_world_t *world);
void handler_camera_move(ecs_world_t *world);
void handler_grid_move(ecs_world_t *world);
void handler_player_input(ecs_world_t *world);
void handler_turncounter_increment(ecs_world_t *world);

#endif // COMPONENTS_H
