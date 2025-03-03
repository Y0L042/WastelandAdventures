#include "ecs_components.h"

/* --- Serializable Components --- */
ECS_COMPONENT_DECLARE(Vector2);
ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Camera2D);
ECS_COMPONENT_DECLARE(CameraFollowTarget);
ECS_COMPONENT_DECLARE(Glyph);

/* --- Other Components --- */

void ecs_define_components(ecs_world_t *world)
{
    /* --- Serializable Components --- */
    ECS_COMPONENT_DEFINE(world, Vector2);
    ecs_struct(world, {
        .entity = ecs_id(Vector2), 
        .members = {
            { .name = "x", .type = ecs_id(ecs_f32_t) }, 
            { .name = "y", .type = ecs_id(ecs_f32_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, Position);
    ecs_struct(world, {
        .entity = ecs_id(Position), 
        .members = {
            { .name = "x", .type = ecs_id(ecs_f32_t) }, 
            { .name = "y", .type = ecs_id(ecs_f32_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, Camera2D);
    ecs_struct(world, {
        .entity = ecs_id(Camera2D), 
        .members = {
            { .name = "offset", .type = ecs_id(Vector2) }, 
            { .name = "target", .type = ecs_id(Vector2) },
            { .name = "rotation", .type = ecs_id(ecs_f32_t) },
            { .name = "zoom", .type = ecs_id(ecs_f32_t) },
        }
    });

    ECS_COMPONENT_DEFINE(world, CameraFollowTarget);
    ecs_struct(world, {
        .entity = ecs_id(CameraFollowTarget), 
        .members = {
            { .name = "target", .type = ecs_id(ecs_u64_t) }
        }
    });

    ECS_COMPONENT_DEFINE(world, Glyph);
    ecs_struct(world, {
        .entity = ecs_id(Glyph),
        .members = {
            { .name = "source_tile_x", .type = ecs_id(ecs_u64_t) },
            { .name = "source_tile_y", .type = ecs_id(ecs_u64_t) }
            // { .name = "color", .type = ecs_id() },
            // { .name = "source_tile_x", .type = ecs_id() },
        }
    });

    /* --- Other Components --- */
}

