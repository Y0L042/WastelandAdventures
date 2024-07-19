#include "turnbasedsystem.h"


void turncomponent_initialize(TurnComponent *tc, TurnManager *tm, int init_initiative)
{
	tc->initiative = init_initiative;
	tc->turn_manager = tm;
	tc->current_turn_state = TURNSTATE_IDLE;
	tc->turn_enter = NULL;
	tc->turn_active_update = NULL;
	tc->turn_exit = NULL;
	tc->turn_idle_update = NULL;
}

void turncomponent_free(TurnComponent *tc)
{
	free(tc);
}

void turncomponent_inc_initiative(TurnComponent *tc, int inc)
{
	tc->initiative += inc;
}

void turncomponent_change_state(TurnComponent *tc, enum TurnState new_state)
{
	if (tc->current_turn_state == TURNSTATE_ACTIVE)
	{
		if (tc->turn_exit != NULL) 
		{
			tc->turn_exit();
		}
	}

	tc->current_turn_state = new_state;

	if (tc->current_turn_state == TURNSTATE_ACTIVE)
	{
		if (tc->turn_enter != NULL)
		{
			tc->turn_enter();
		}
	}
}

void turncomponent_process_state(TurnComponent *tc, double delta)
{
	switch (tc->current_turn_state)
	{
		case TURNSTATE_IDLE:
			if (tc->turn_idle_update != NULL)
			{
				tc->turn_idle_update(delta);
			}
			break;
		case TURNSTATE_ACTIVE:
			if (tc->turn_active_update != NULL)
			{
				tc->turn_active_update(delta);
			}
			break;
		default:
			break;
	}
}

void turncomponent_start_turn(TurnComponent *tc)
{
	turncomponent_change_state(tc, TURNSTATE_ACTIVE);
}

void turncomponent_end_turn(TurnComponent *tc, int inc)
{
	turncomponent_inc_initiative(tc, inc);
	turncomponent_change_state(tc, TURNSTATE_IDLE);
}



void turnmanager_initialize(TurnManager *tm)
{
	cvec_void_init(&(tm->tc_ptr_registry));
}

void turnmanager_add_turncomponent(TurnManager *tm, TurnComponent *tc)
{
	cvec_void_add_item(&tm->tc_ptr_registry, tc);
}

void turnmanager_remove_turncomponent(TurnManager *tm, TurnComponent *tc)
{
	for (int i = 0; i < tm->tc_ptr_registry.count; i++)
	{
		if (tc == tm->tc_ptr_registry.data[i])
		{
			cvec_void_remove_idx(&tm->tc_ptr_registry, i);
			return;
		}
	}
}

void turnmanager_next_turn(TurnManager *tm, TurnComponent *tc, int inc)
{
	turncomponent_end_turn(tc, inc);
	// sort tc_reg by initiative and last turn
	tm->active_tc_idx = (tm->active_tc_idx++) % tm->tc_ptr_registry.count;
	TurnComponent *active_tc = tm->tc_ptr_registry.data[tm->active_tc_idx];
	turncomponent_start_turn(active_tc);
}

void turnmanager_execute_turn_statemachine(TurnManager *tm, double delta)
{
	for (int i = 0; i < tm->tc_ptr_registry.count; i++)
	{
        TurnComponent *tc = (TurnComponent *)tm->tc_ptr_registry.data[i];
		turncomponent_process_state(tc, delta);
	}
}

