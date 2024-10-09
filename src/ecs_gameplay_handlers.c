#include "ecs_gameplay_handlers.h"

#include "ecs_components.h"

/* --- Queries --- */
static ecs_query_t *query_initialize_position2d = NULL;



void create_gameplay_queries(ecs_world_t *world)
{
    query_initialize_position2d = 
        ecs_query(world, { .terms = {
            {ecs_id(Position2D)}
        }});
}

void free_gameplay_queries(ecs_world_t *world)
{
    ecs_query_fini(query_initialize_position2d);
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

