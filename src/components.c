#include "components.h"

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(GridPosition);
ECS_COMPONENT_DECLARE(GridVelocity);
ECS_COMPONENT_DECLARE(GridComponent);
ECS_COMPONENT_DECLARE(Glyph);
ECS_COMPONENT_DECLARE(CameraComponent);
ECS_COMPONENT_DECLARE(TurnComponent);

ECS_ENTITY_DECLARE(TAG_Player);
ECS_ENTITY_DECLARE(TAG_TurnActive);
ECS_ENTITY_DECLARE(TAG_TurnIdle);

void create_components(ecs_world_t *world)
{
	ECS_COMPONENT_DEFINE(world, Position);
	ECS_COMPONENT_DEFINE(world, Velocity);
	ECS_COMPONENT_DEFINE(world, GridPosition);
	ECS_COMPONENT_DEFINE(world, GridVelocity);
	ECS_COMPONENT_DEFINE(world, GridComponent);
	ECS_COMPONENT_DEFINE(world, Glyph);
	ECS_COMPONENT_DEFINE(world, CameraComponent);
    ECS_COMPONENT_DEFINE(world, TurnComponent);

    ECS_ENTITY_DEFINE(world, TAG_Player);
    ECS_ENTITY_DEFINE(world, TAG_TurnActive);
    ECS_ENTITY_DEFINE(world, TAG_TurnIdle);
}



void create_queries(ecs_world_t *world)
{
    //ecs_query_t query_draw_glyph;
    //query_draw_glyph = ecs_query_init(world, &(ecs_query_desc_t) {
    //    .terms = {
    //        {ecs_id(Glyph) },
    //        {ecs_id(Position) },
    //        { 0 }  
    //    }
    //}); 
}

void handler_draw_glyph(ecs_world_t *world)
{
    ecs_query_t *query_draw_glyph = ecs_query(world, {
        .filter.terms = {
            {ecs_id(Glyph) },
            {ecs_id(Position) }
        }
    }); 
    ecs_iter_t it = ecs_query_iter(world, query_draw_glyph);
    while (ecs_query_next(&it))
    {
        Glyph *g = ecs_field(&it, Glyph, 1);
        Position *p = ecs_field(&it, Position, 2);

        for (int i = 0; i < it.count; i++)
        {
            Glyph glyph = g[i];
            glyph_draw(&glyph, p[i].x, p[i].y, RAYWHITE);
        }
    }

    ecs_query_fini(query_draw_glyph);
}
