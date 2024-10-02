#ifndef TURNBASEDSYSTEM_H
#define TURNBASEDSYSTEM_H

#include <raylib.h>

#include "cvec.h"
#include "flecs.h"

#include "components.h"

#define TURNCOMP_ALIAS_LEN 15

enum TurnState {
	TURNSTATE_ACTIVE,
	TURNSTATE_IDLE
};

typedef struct TAG_TurnActive { int empty; } TAG_TurnActive;
typedef struct TAG_TurnIdle { int empty; } TAG_TurnIdle;
typedef struct TurnEnter { int empty; } TurnEnter;
typedef struct TurnProcess { int empty; } TurnProcess;
typedef struct TurnExit { int empty; } TurnExit;

struct TurnManager; // Forward Declaration
typedef struct TurnComponentData {
    /* Known at compile-time */
	int enable;
	int initiative;	
	char alias[TURNCOMP_ALIAS_LEN];
     
    /* Known at runtime */
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

typedef struct TAG_TCEnable { int empty; } TAG_TCEnable;

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
void turncomponentdata_enable(TurnComponentData *tc_d);
void turncomponentdata_disable(TurnComponentData *tc_d);

void turnmanager_initialize(TurnManager *tm, ecs_world_t *world);
TurnComponentData* turnmanager_create_turncomponent(TurnManager *tm, ecs_entity_t entity);
void turnmanager_end_turn(TurnManager *tm, int inc);
void turnmanager_print_turn_queue(TurnManager *tm);
void turnmanager_enable_tc(TurnManager *tm, ecs_entity_t entity);
void turnmanager_disable_tc(TurnManager *tm, ecs_entity_t entity);
void turnmanager_set_component_alias(TurnManager *tm, ecs_entity_t entity, const char *alias);

void turncounter_create(TurnManager *tm, ecs_world_t *world);

#endif // TURNBASEDSYSTEM_H
