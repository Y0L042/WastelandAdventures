#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include <flecs.h>
#include <raylib.h>

/* --- Component Sources --- */
#include "ecs_components_gameplay.h"
#include "camera.h"
#include "glyph.h"

/* ------------------------- */

/* --- Component Forward Declarations --- */
/* --- Serializable Components --- */
extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Camera2D);
extern ECS_COMPONENT_DECLARE(CameraFollowTarget);
extern ECS_COMPONENT_DECLARE(Glyph);

/* --- Other Components --- */




/* --- Functions --- */
void ecs_define_components(ecs_world_t *world);



#endif /* ECS_COMPONENTS_H */
