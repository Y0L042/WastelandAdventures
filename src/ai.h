#ifndef AI_H
#define AI_H

#include "flecs.h"

typedef struct AIComponent AIComponent;
typedef struct AIComponent {
	ecs_world_t *world;
	ecs_entity_t entity;
	int (*ai_main)(AIComponent *);
} AIComponent;

#endif // AI_H
