#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <raylib.h>
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

typedef struct GridComponent{
	Grid *grid;
	Tile *tile;
} GridComponent;
extern ECS_COMPONENT_DECLARE(GridComponent);

extern ECS_COMPONENT_DECLARE(Glyph);

typedef struct CameraComponent{
	Camera2D camera;
	ecs_entity_t target_entity;
	ecs_entity_t previous_target_entity;
} CameraComponent;
extern ECS_COMPONENT_DECLARE(CameraComponent);

extern ECS_COMPONENT_DECLARE(TurnComponent);

extern ecs_entity_t TAG_Player;



void create_components(ecs_world_t *world);


#endif // COMPONENTS_H
