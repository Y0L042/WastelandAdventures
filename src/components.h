#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <raylib.h>
#include <raymath.h>
#include "log.h"
#include "flecs.h"
#include "grid.h"
#include "glyph.h"
#include "turnbasedsystem.h"
#include "pathfinding.h"

typedef Vector2 Position;
extern ECS_COMPONENT_DECLARE(Position);

typedef Vector2 Velocity;
extern ECS_COMPONENT_DECLARE(Velocity);

extern ECS_COMPONENT_DECLARE(GridPosition);

typedef Vector2 GridVelocity;
extern ECS_COMPONENT_DECLARE(GridVelocity);

extern ECS_COMPONENT_DECLARE(Glyph);
extern ECS_COMPONENT_DECLARE(GlyphFade);
typedef struct LeaveGlyphGhost { int fade_time; int x; int y; } LeaveGlyphGhost;
extern ECS_COMPONENT_DECLARE(LeaveGlyphGhost);
typedef struct GhostWhenMoving { int fade_time; } GhostWhenMoving;
extern ECS_COMPONENT_DECLARE(GhostWhenMoving);

typedef struct CameraComponent{
	Camera2D camera;
	ecs_entity_t target_entity;
	ecs_entity_t previous_target_entity;
} CameraComponent;
extern ECS_COMPONENT_DECLARE(CameraComponent);

extern ECS_COMPONENT_DECLARE(TurnComponent);
typedef struct TurnCountComponent TurnCountComponent; // Forward Declaration
extern ECS_COMPONENT_DECLARE(TurnCountComponent);
extern ECS_COMPONENT_DECLARE(TAG_TCEnable);
extern ECS_COMPONENT_DECLARE(TurnEnter);
extern ECS_COMPONENT_DECLARE(TurnProcess);
extern ECS_COMPONENT_DECLARE(TurnExit);

extern ECS_COMPONENT_DECLARE(PathComponent);

typedef struct NPCTarget {
    ecs_entity_t target;
} NPCTarget;
extern ECS_COMPONENT_DECLARE(NPCTarget);

typedef struct HealthComponent {
	int health;
	int _initial_health;
} HealthComponent;
extern ECS_COMPONENT_DECLARE(HealthComponent);

typedef struct TAG_Player{ int empty; } TAG_Player;
extern ECS_COMPONENT_DECLARE(TAG_Player);

extern ECS_COMPONENT_DECLARE(TAG_TurnActive);

extern ECS_COMPONENT_DECLARE(TAG_TurnIdle);


void create_components(ecs_world_t *world);




// Handlers

void create_queries(ecs_world_t *world);

void handler_glyph_draw(ecs_world_t *world);
void handler_glyph_ghost_spawn(ecs_world_t *world);
void handler_glyph_fade(ecs_world_t *world, double delta);
void handler_camera_move(ecs_world_t *world);
void handler_grid_move(ecs_world_t *world);
void handler_player_input(ecs_world_t *world);
void handler_pathfinding(ecs_world_t *world);
void handler_turncounter_increment(ecs_world_t *world);
void handler_draw_health_ui(ecs_world_t *world);

#endif // COMPONENTS_H
