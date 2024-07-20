#ifndef TURNBASEDSYSTEM_H
#define TURNBASEDSYSTEM_H

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
    ecs_world_t *world;
    ecs_entity_t entity_id;
	struct TurnManager *turn_manager;
	enum TurnState current_turn_state;
} TurnComponent;

typedef struct TurnManager {
	int active_tc_idx;
	CVecVoid tc_ptr_registry; // CVecVoid<*TurnComponent>
} TurnManager;

void turncomponent_initialize(TurnComponent *tc, TurnManager *tm, ecs_entity_t entity_id, ecs_world_t *world, int init_initiative);
void turncomponent_free(TurnComponent *tc);
void turncomponent_inc_initiative(TurnComponent *tc, int inc);
int turncomponent_compare_initiatives(const void *a, const void *b);
void turncomponent_change_state(TurnComponent *tc, enum TurnState new_state);
void turncomponent_start_turn(TurnComponent *tc);
void turncomponent_end_turn(TurnComponent *tc, int inc);

void turnmanager_initialize(TurnManager *tm);
void turnmanager_add_turncomponent(TurnManager *tm, TurnComponent *tc);
void turnmanager_remove_turncomponent(TurnManager *tm, TurnComponent *tc);
void turnmanager_next_turn(TurnManager *tm, TurnComponent *tc, int inc);

#endif // TURNBASEDSYSTEM_H
