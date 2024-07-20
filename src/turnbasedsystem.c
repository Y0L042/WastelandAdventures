#include "turnbasedsystem.h"


void turncomponent_initialize(TurnComponent *tc, TurnManager *tm, ecs_entity_t entity_id, ecs_world_t *world, int init_initiative)
{
	tc->initiative = init_initiative;
    tc->world = world;
    tc->entity_id = entity_id;
	tc->turn_manager = tm;
	tc->current_turn_state = TURNSTATE_IDLE;
}

void turncomponent_free(TurnComponent *tc)
{
	free(tc);
}

void turncomponent_inc_initiative(TurnComponent *tc, int inc)
{
	tc->initiative += inc;
}

int turncomponent_compare_initiatives(const void *a, const void *b)
{
    TurnComponent *tc_a = *(TurnComponent**)a;
    TurnComponent *tc_b = *(TurnComponent**)b;
    int result = tc_a->initiative - tc_b->initiative;
    return result;
}

void turncomponent_change_state(TurnComponent *tc, enum TurnState new_state)
{
    tc->current_turn_state = new_state;
}

void turncomponent_start_turn(TurnComponent *tc)
{
    ecs_remove(tc->world, tc->entity_id, TAG_TurnIdle);
    ecs_add(tc->world, tc->entity_id, TAG_TurnActive);
    turncomponent_change_state(tc, TURNSTATE_ACTIVE);
    printf("\n\n DEBUG 1 \n\n");
}

void turncomponent_end_turn(TurnComponent *tc, int inc)
{
    ecs_remove(tc->world, tc->entity_id, TAG_TurnActive);
	ecs_add(tc->world, tc->entity_id, TAG_TurnIdle);
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
			cvec_void_remove_idx_noshrink(&tm->tc_ptr_registry, i);
			return;
		}
	}
}

void turnmanager_next_turn(TurnManager *tm, TurnComponent *tc, int inc)
{
	turncomponent_end_turn(tc, inc);
	
    for (int i = 1; i < tm->tc_ptr_registry.count; i++) 
    {
        TurnComponent *key = tm->tc_ptr_registry.data[i];
        int j = i - 1;
        while (j >= 0 && turncomponent_compare_initiatives(tm->tc_ptr_registry.data[j], key) < 0) 
        {
            tm->tc_ptr_registry.data[j + 1] = tm->tc_ptr_registry.data[j];
            j = j - 1;
        }
        tm->tc_ptr_registry.data[j + 1] = key;
    }

	tm->active_tc_idx = (tm->active_tc_idx++) % tm->tc_ptr_registry.count;
	TurnComponent *active_tc = tm->tc_ptr_registry.data[tm->active_tc_idx];
	turncomponent_start_turn(active_tc);
}


