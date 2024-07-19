#include "components.h"

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(GridPosition);
ECS_COMPONENT_DECLARE(GridVelocity);
ECS_COMPONENT_DECLARE(GridComponent);
ECS_COMPONENT_DECLARE(Glyph);
ECS_COMPONENT_DECLARE(CameraComponent);

ecs_entity_t TAG_Player;

void create_components(ecs_world_t *world)
{
	ECS_COMPONENT_DEFINE(world, Position);
	ECS_COMPONENT_DEFINE(world, Velocity);
	ECS_COMPONENT_DEFINE(world, GridPosition);
	ECS_COMPONENT_DEFINE(world, GridVelocity);
	ECS_COMPONENT_DEFINE(world, GridComponent);
	ECS_COMPONENT_DEFINE(world, Glyph);
	ECS_COMPONENT_DEFINE(world, CameraComponent);



	TAG_Player = ecs_new_id(world);
}


