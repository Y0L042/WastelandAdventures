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
typedef struct TurnComponentData {
	int initiative;	
	double last_turn_time;
	enum TurnState current_turn_state;
	struct TurnManager *turn_manager;
    ecs_world_t *world;
    ecs_entity_t entity_id;
	ecs_ref_t *tc_ref;
} TurnComponentData;

typedef struct TurnComponent {
	TurnComponentData *tc_d;
} TurnComponent;

typedef struct TurnManager {
	int active_tc_idx;
	int tracked_tc_count;
	ecs_world_t *world;
	ecs_entity_t turn_counter;
	ecs_ref_t *turn_counter_ref;
	CVecVoid tc_refs; // CVecVoid<ecs_ref_t *<TurnComponent *>>
	CVecVoid tc_datas; // CVecVoid<TurnComponentData *>
} TurnManager;

typedef struct TurnCountComponent { 
	uint16_t count; 
} TurnCountComponent;

void turncomponentdata_initialize(
		TurnComponentData *tc_d, 
		TurnManager *tm, 
		ecs_entity_t entity_id, 
		ecs_world_t *world, 
		int init_initiative
	);
void turncomponentdata_free(TurnComponentData *tc_d);
void turncomponentdata_inc_initiative(TurnComponentData *tc_d, int inc);
int turncomponentdata_compare_initiatives(TurnComponentData *tc_d_a, TurnComponentData *tc_d_b);
void turncomponentdata_change_state(TurnComponentData *tc_d, enum TurnState new_state);
void turncomponentdata_start_turn(TurnComponentData *tc_d);
void turncomponentdata_end_turn(TurnComponentData *tc_d, int inc);

void turnmanager_initialize(TurnManager *tm, ecs_world_t *world);
TurnComponentData* turnmanager_create_turncomponent(TurnManager *tm, ecs_entity_t entity);
//void turnmanager_remove_turncomponent(TurnManager *tm, TurnComponent *tc);
void turnmanager_end_turn(TurnManager *tm, int inc);
void turnmanager_print_turn_queue(TurnManager *tm);

void turncounter_create(TurnManager *tm, ecs_world_t *world);

#endif // TURNBASEDSYSTEM_H
