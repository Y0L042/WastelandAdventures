#ifndef TURNBASEDSYSTEM_H
#define TURNBASEDSYSTEM_H

#include <raylib.h>
#include "log.h"

#include "cvec.h"
#include "flecs.h"

#include "components.h"

enum TurnState {
	TURNSTATE_ACTIVE,
	TURNSTATE_IDLE
};

struct TurnManager; // Forward Declaration
typedef struct TurnComponent {
    char *alias;
	int initiative;	
	double last_turn_time;
    ecs_world_t *world;
    ecs_entity_t entity_id;
	struct TurnManager *turn_manager;
	enum TurnState current_turn_state;
} TurnComponent;

typedef struct TurnManager {
	int active_tc_idx;
	ecs_world_t *world;
	ecs_entity_t turn_counter;
	CVecVoid tc_ptr_registry; // CVecVoid<*TurnComponent>
} TurnManager;

typedef struct TurnCountComponent { 
	uint16_t count; 
} TurnCountComponent;
//extern ecs_entity_t turn_counter;

void turncomponent_initialize(TurnComponent *tc, TurnManager *tm, ecs_entity_t entity_id, ecs_world_t *world, int init_initiative);
void turncomponent_free(TurnComponent *tc);
int turncomponent_inc_initiative(TurnComponent *tc, int inc);
int turncomponent_compare_initiatives(const void *a, const void *b);
void turncomponent_change_state(TurnComponent *tc, enum TurnState new_state);
void turncomponent_start_turn(TurnComponent *tc);
void turncomponent_end_turn(TurnComponent *tc, int inc);

void turnmanager_initialize(TurnManager *tm, ecs_world_t *world);
void turnmanager_add_turncomponent(TurnManager *tm, TurnComponent *tc);
void turnmanager_remove_turncomponent(TurnManager *tm, TurnComponent *tc);
void turnmanager_next_turn(TurnManager *tm, TurnComponent *tc, int inc);
void turnmanager_print_turn_queue(TurnManager *tm);

void turncounter_create(TurnManager *tm, ecs_world_t *world);

#endif // TURNBASEDSYSTEM_H
