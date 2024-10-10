#include "ecs_components.h"

/* --- Serializable Components --- */
ECS_COMPONENT_DECLARE(Position2D);

/* --- Other Components --- */
ECS_COMPONENT_DECLARE(Camera2DComponent);

void ecs_define_components(ecs_world_t *world)
{
    /* --- Serializable Components --- */
    ECS_COMPONENT_DEFINE(world, Position2D);
    ecs_struct(world, {
        .entity = ecs_id(Position2D), 
        .members = {
            { .name = "x", .type = ecs_id(ecs_f32_t) }, 
            { .name = "y", .type = ecs_id(ecs_f32_t) }
        }
    });

    /* --- Other Components --- */
    ECS_COMPONENT_DEFINE(world, Camera2DComponent);
}
