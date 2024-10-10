#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include <flecs.h>
#include <raylib.h>

/* --- Component Sources --- */
#include "components_gameplay.h"
#include "camera.h"

/* ------------------------- */

/* --- Component Forward Declarations --- */
/* --- Serializable Components --- */
extern ECS_COMPONENT_DECLARE(Position2D);

/* --- Other Components --- */
extern ECS_COMPONENT_DECLARE(Camera2DComponent);




/* --- Functions --- */
void ecs_define_components(ecs_world_t *world);



#endif /* ECS_COMPONENTS_H */
