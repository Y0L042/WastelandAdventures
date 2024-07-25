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
	CVecVoid tc_refs; // CVecVoid<ecs_ref_t *>
} TurnManager;

typedef struct TurnCountComponent { 
	uint16_t count; 
} TurnCountComponent;

void turnmanager_initialize(TurnManager *tm, ecs_world_t *world);
void turnmanager_add_turncomponent(TurnManager *tm, ecs_ref_t *tc_ref, ecs_entity_t entity);
void turnmanager_remove_turncomponent(TurnManager *tm, ecs_ref_t *tc_ref);
void turnmanager_next_turn(TurnManager *tm, ecs_ref_t *tc_ref, int inc);
void turnmanager_print_turn_queue(TurnManager *tm);
void turnmanager_inc_tc_initiative(TurnManager *tm, ecs_ref_t *tc_ref, int inc);
int  turnmanager_compare_tc(TurnManager *tm, ecs_ref_t *tc_ref_a, ecs_ref_t *tc_ref_b);
void turnmanager_change_tc_state(TurnManager *tm, enum TurnState new_state);
void turnmanager_start_turn(TurnManager *tm, ecs_ref_t *tc_ref);
void turnmanager_end_turn(TurnManager *tm, ecs_ref_t *tc_ref, int inc);

void turncounter_create(TurnManager *tm, ecs_world_t *world);

#endif // TURNBASEDSYSTEM_H
