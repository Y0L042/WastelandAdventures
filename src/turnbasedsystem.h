#ifndef TURNBASEDSYSTEM_H
#define TURNBASEDSYSTEM_H

#include "cvec.h"

enum TurnState {
	TURNSTATE_ACTIVE,
	TURNSTATE_IDLE
};

struct TurnManager; // Forward Declaration
typedef struct TurnComponent {
	int initiative;	
	struct TurnManager *turn_manager;
	enum TurnState current_turn_state;
	void (*turn_enter)(void);
	void (*turn_active_update)(double delta);
	void (*turn_exit)(void);
	void (*turn_idle_update)(double delta);
} TurnComponent;

typedef struct TurnManager {
	int active_tc_idx;
	CVecVoid tc_ptr_registry; // CVecVoid<*TurnComponent>
	void (*proc_turn_enter)(void);
	void (*proc_turn_active_update)(double delta);
	void (*proc_turn_exit)(void);
	void (*proc_turn_idle_update)(double delta);
} TurnManager;

void turncomponent_initialize(TurnComponent *tc, TurnManager *tm, int init_initiative);
void turncomponent_free(TurnComponent *tc);
void turncomponent_inc_initiative(TurnComponent *tc, int inc);
void turncomponent_change_state(TurnComponent *tc, enum TurnState new_state);
void turncomponent_process_state(TurnComponent *tc, double delta);
void turncomponent_start_turn(TurnComponent *tc);
void turncomponent_end_turn(TurnComponent *tc, int inc);

void turnmanager_initialize(TurnManager *tm);
void turnmanager_add_turncomponent(TurnManager *tm, TurnComponent *tc);
void turnmanager_remove_turncomponent(TurnManager *tm, TurnComponent *tc);
void turnmanager_next_turn(TurnManager *tm, TurnComponent *tc, int inc);
void turnmanager_execute_turn_statemachine(TurnManager *tm, double delta);

#endif // TURNBASEDSYSTEM_H
