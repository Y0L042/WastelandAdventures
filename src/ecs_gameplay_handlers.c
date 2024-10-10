#include "ecs_gameplay_handlers.h"

#include "ecs_components.h"

/* --- Queries --- */
static ecs_query_t *query_initialize_position2d = NULL;
static ecs_query_t *query_initialize_camera2dcomponent = NULL;



void create_gameplay_queries(ecs_world_t *world)
{
    query_initialize_position2d = 
        ecs_query(world, { .terms = {
            {ecs_id(Position2D)}
        }});

    query_initialize_camera2dcomponent = 
        ecs_query(world, { .terms = {
            {ecs_id(Camera2DComponent)}
        }});
}

void free_gameplay_queries(ecs_world_t *world)
{
    ecs_query_fini(query_initialize_position2d);
    ecs_query_fini(query_initialize_camera2dcomponent);
}

void handler_initialize_position2d(ecs_world_t *world)
{
    ecs_iter_t it = ecs_query_iter(world, query_initialize_position2d);
    while (ecs_query_next(&it))
    {
        Position2D *p = ecs_field(&it, Position2D, 0);

        for (int i = 0; i < it.count; i++)
        {
            /* Initialize Position2D, maybe based on given GridPosition? */
        }
    }
}

void handler_initialize_camera2d(ecs_world_t *world)
{
    ecs_iter_t it = ecs_query_iter(world, query_initialize_camera2dcomponent);
    while (ecs_query_next(&it))
    {
        Camera2DComponent *cc = ecs_field(&it, Camera2DComponent, 0);

        for (int i = 0; i < it.count; i++)
        {
            /* Initialize Camera2DComponent */
        }
    }
}

