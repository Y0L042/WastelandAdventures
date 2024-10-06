#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include <raylib.h>
#include <flecs.h>

#include "components_gameplay.h"

/* --- Component Forward Declarations --- */
/* --- Serializable Components --- */
extern ECS_COMPONENT_DECLARE(Position2D);

/* --- Other Components --- */




/* --- Functions --- */
void ecs_define_components(ecs_world_t *world);



#endif /* ECS_COMPONENTS_H */
