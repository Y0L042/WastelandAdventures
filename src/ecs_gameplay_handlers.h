#ifndef ECS_GAMEPLAY_HANDLERS_H
#define ECS_GAMEPLAY_HANDLERS_H

#include <flecs.h>

void create_gameplay_queries(ecs_world_t *world);
void free_gameplay_queries(ecs_world_t *world);

void handler_initialize_position2d(ecs_world_t *world);
void handler_initialize_camera2d(ecs_world_t *world);



#endif /* ECS_GAMEPLAY_HANDLERS_H */
