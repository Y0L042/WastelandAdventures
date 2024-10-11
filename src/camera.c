#include "camera.h"

#include <raylib.h>
#include <flecs.h>
#include "ecs_components.h"

/* --- Public Variables --- */
ecs_world_t *active_camera_world;
ecs_ref_t *active_camera2d_ref;

/* --- Private Variables --- */
static int is_active_camera_set = 0;



ecs_ref_t *create_camera2d(ecs_world_t *world)
{
    Camera2D rl_camera = { .zoom = 1 };

    ecs_entity_t camera = ecs_new(world);
    ecs_add(world, camera, Camera2D);
    Camera2D *c = ecs_get_mut(world, camera, Camera2D);
    *c = rl_camera;

	ecs_ref_t *camera_ref = (ecs_ref_t *)malloc(sizeof(ecs_ref_t)); 
    *camera_ref = ecs_ref_init(world, camera, Camera2D);

    return camera_ref;
}

ecs_ref_t *create_camera2d_pro(ecs_world_t *world, Camera2D params)
{
    Camera2D rl_camera = params;

    ecs_entity_t camera = ecs_new(world);
    ecs_add(world, camera, Camera2D);
    Camera2D *c = ecs_get_mut(world, camera, Camera2D);
    *c = rl_camera;

	ecs_ref_t *camera_ref = (ecs_ref_t *)malloc(sizeof(ecs_ref_t)); 
    *camera_ref = ecs_ref_init(world, camera, Camera2D);

    return camera_ref;
}

void camera2dcomponent_free_ref(ecs_ref_t *camera2d_ref)
{
    free(camera2d_ref);
}

void set_active_camera2d(ecs_world_t *world, ecs_ref_t *camera2d_ref)
{
    is_active_camera_set = 1;

	active_camera2d_ref = camera2d_ref;
	active_camera_world = world;
}

int is_active_camera2d_set(void)
{
    return is_active_camera_set;
}

void camera2dcomponent_set_params(ecs_ref_t *camera2d_ref, Camera2D params)
{
    if (!active_camera_world) { return; }

    Camera2D *c = ecs_ref_get(active_camera_world, camera2d_ref, Camera2D);
    if (c) {
        *c = params;
    }
}

void camera2dcomponent_set_offset(ecs_ref_t *camera2d_ref, float x, float y)
{
    if (!active_camera_world) { return; }

    Camera2D *c = ecs_ref_get(active_camera_world, camera2d_ref, Camera2D);
    if (c) {
        c->offset = (Vector2){ x, y };
    }
}

void camera2dcomponent_set_target(ecs_ref_t *camera2d_ref, float x, float y)
{
    if (!active_camera_world) { return; }

    Camera2D *c = ecs_ref_get(active_camera_world, camera2d_ref, Camera2D);
    if (c) {
        c->target = (Vector2){ x, y };
    }
}

void camera2dcomponent_set_rotation(ecs_ref_t *camera2d_ref, float rotation)
{
    if (!active_camera_world) { return; }

    Camera2D *c = ecs_ref_get(active_camera_world, camera2d_ref, Camera2D);
    if (c) {
        c->rotation = rotation;
    }
}

void camera2dcomponent_set_zoom(ecs_ref_t *camera2d_ref, float zoom)
{
    if (!active_camera_world) { return; }

    Camera2D *c = ecs_ref_get(active_camera_world, camera2d_ref, Camera2D);
    if (c) {
        c->zoom = zoom;
    }
}

