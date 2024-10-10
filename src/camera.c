#include "camera.h"

#include <raylib.h>
#include <flecs.h>
#include "ecs_components.h"

static int is_active_camera_set = 0;

ecs_world_t *active_camera_world;
ecs_ref_t *active_camera2d_ref;

ecs_ref_t *create_camera2d(ecs_world_t *world)
{
    ecs_entity_t camera = ecs_new(world);
    Camera2D rl_camera = { .zoom = 1 };
    ecs_set(world, camera, Camera2DComponent, { .camera = rl_camera });
	ecs_ref_t *camera_ref = (ecs_ref_t *)malloc(sizeof(ecs_ref_t)); 
    *camera_ref = ecs_ref_init(world, camera, Camera2DComponent);

    return camera_ref;
}

ecs_ref_t *create_camera2d_pro(ecs_world_t *world, Camera2D params)
{
    ecs_entity_t camera = ecs_new(world);
    Camera2D rl_camera = params;
    ecs_set(world, camera, Camera2DComponent, { .camera = rl_camera });
	ecs_ref_t *camera_ref = (ecs_ref_t *)malloc(sizeof(ecs_ref_t)); 
    *camera_ref = ecs_ref_init(world, camera, Camera2DComponent);

    return camera_ref;
}

void free_camera2d(ecs_ref_t *camera2d_ref)
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

void camera2d_set_rl_camera(ecs_ref_t *camera2d_ref, Camera2D params)
{
    Camera2D *c = GET_RL_CAMERA2D_FROM_REF(active_camera_world, camera2d_ref);
    *c = params;
}

void camera2dcomponent_set_offset(ecs_ref_t *camera2d_ref, float x, float y)
{
    Camera2D *c = GET_RL_CAMERA2D_FROM_REF(active_camera_world, camera2d_ref);
    if (c) {
        c->offset = (Vector2){ x, y };
    }
}

void camera2dcomponent_set_target(ecs_ref_t *camera2d_ref, float x, float y)
{
    Camera2D *c = GET_RL_CAMERA2D_FROM_REF(active_camera_world, camera2d_ref);
    if (c) {
        c->target = (Vector2){ x, y };
    }
}

void camera2dcomponent_set_rotation(ecs_ref_t *camera2d_ref, float rotation)
{
    Camera2D *c = GET_RL_CAMERA2D_FROM_REF(active_camera_world, camera2d_ref);
    if (c) {
        c->rotation = rotation;
    }
}

void camera2dcomponent_set_zoom(ecs_ref_t *camera2d_ref, float zoom)
{
    Camera2D *c = GET_RL_CAMERA2D_FROM_REF(active_camera_world, camera2d_ref);
    if (c) {
        c->zoom = zoom;
    }
}

