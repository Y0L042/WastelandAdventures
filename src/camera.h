#ifndef CAMERA2D_H
#define CAMERA2D_H

#include <raylib.h>
#include <flecs.h>

#include "dray.h"

#define GET_RL_CAMERA2D_FROM_REF(_world, _camera2d_ref_ptr) \
        (&(ecs_ref_get(_world, _camera2d_ref_ptr, Camera2DComponent)->camera))

extern ecs_world_t *active_camera_world;
extern ecs_ref_t *active_camera2d_ref;

typedef struct Camera2DComponent {
    Camera2D camera;
    ecs_entity_t target_entity;
    DRay target_stack;
} Camera2DComponent;

ecs_ref_t *create_camera2d(ecs_world_t *world);
ecs_ref_t *create_camera2d_pro(ecs_world_t *world, Camera2D params);
void free_camera2d(ecs_ref_t *camera2d_ref);
void set_active_camera2d(ecs_world_t *world, ecs_ref_t *camera2d_ref);
int is_active_camera2d_set(void);
void camera2d_set_rl_camera(ecs_ref_t *camera2d_ref, Camera2D params);
void camera2dcomponent_set_offset(ecs_ref_t *camera2d_ref, float x, float y);
void camera2dcomponent_set_target(ecs_ref_t *camera2d_ref, float x, float y);
void camera2dcomponent_set_rotation(ecs_ref_t *camera2d_ref, float rotation);
void camera2dcomponent_set_zoom(ecs_ref_t *camera2d_ref, float zoom);


#endif /* CAMERA2D_H */

















#ifndef CAMERA3D_H
#define CAMERA3D_H

#include <raylib.h>
#include <flecs.h>

#include "dray.h"

typedef struct Camera3DComponent {
    Camera3D camera;
    ecs_entity_t target_entity;
    DRay target_stack;
} Camera3DComponent;


#endif /* CAMERA3D_H */
