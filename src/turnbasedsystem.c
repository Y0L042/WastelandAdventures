#include "turnbasedsystem.h"
#include <math.h>

//ecs_entity_t turn_counter;

void turncomponent_initialize(TurnComponent *tc, TurnManager *tm, ecs_entity_t entity_id, ecs_world_t *world, int init_initiative)
{
	tc->initiative = init_initiative;
    tc->world = world;
    tc->entity_id = entity_id;
	tc->turn_manager = tm;
	tc->current_turn_state = TURNSTATE_IDLE;
	turnmanager_add_turncomponent(tm, tc);
}

void turncomponent_free(TurnComponent *tc)
{
	free(tc);
}

int turncomponent_inc_initiative(TurnComponent *tc, int inc)
{
	//printf("Entity: %d\tinitial i: %d , inc: %d", (uint32_t)tc->entity_id, tc->initiative, inc);
	TurnComponent *temp_tc = ecs_get_mut(tc->world, tc->entity_id, TurnComponent);
	temp_tc->initiative += inc;
	//printf(" result: %d\n", tc->initiative);
	return temp_tc->initiative;
}

int turncomponent_compare_initiatives(const void *a, const void *b)
{
    TurnComponent *tc_a = *(TurnComponent**)a;
    TurnComponent *tc_b = *(TurnComponent**)b;
    int result = tc_a->initiative - tc_b->initiative;
	if (result == 0)
	{
		double time_diff = tc_a->last_turn_time - tc_b->last_turn_time;
		result = time_diff < 0 ? -1 : 1;
	}
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
}

void turncomponent_end_turn(TurnComponent *tc, int inc)
{
	if (tc->world == NULL)
	{
		log_error("Entity's world is NULL!\n");
	}
    ecs_remove(tc->world, tc->entity_id, TAG_TurnActive);
	ecs_add(tc->world, tc->entity_id, TAG_TurnIdle);
	tc->last_turn_time = GetTime();
	turncomponent_inc_initiative(tc, inc);
	turncomponent_change_state(tc, TURNSTATE_IDLE);
}



void turnmanager_initialize(TurnManager *tm, ecs_world_t *world)
{
	tm->world = world;
	cvec_void_init(&(tm->tc_ptr_registry));
	turncounter_create(tm, world);
    TurnComponent *tc = ecs_get_mut(world, tm->turn_counter, TurnComponent);
    turncomponent_start_turn(tc);
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
		
    //for (int i = 1; i < tm->tc_ptr_registry.count; i++) 
    //{
    //    TurnComponent *key = tm->tc_ptr_registry.data[i];
    //    int j = i - 1;
    //    while (
	//		j >= 0 
	//		&& turncomponent_compare_initiatives(
	//			tm->tc_ptr_registry.data[j], key
	//		) > 0
	//	) 
    //    {
    //        tm->tc_ptr_registry.data[j + 1] = tm->tc_ptr_registry.data[j];
    //        j = j - 1;
    //    }
    //    tm->tc_ptr_registry.data[j + 1] = key;
    //}
	int min_initiative_idx = 0; 
	int min_initiative = -1;
	for (int i = 0; i < tm->tc_ptr_registry.count; i++)
	{
		log_debug("TurnComponent DEBUG");
		/*
		*	Loop thru each turncomponent. If that component has a lower initiative,
		*	or older (smaller) last_turn_time, set its idx as min_initiative_idx.
		*	It will be the next component to be TurnActive.
		*/
		TurnComponent *index_turncomp = (TurnComponent *)(tm->tc_ptr_registry.data[i]);
		if (index_turncomp->initiative <= min_initiative || min_initiative < 0)
		{
			int init_comp = turncomponent_compare_initiatives(
					index_turncomp,
					tm->tc_ptr_registry.data[min_initiative_idx]
				);
			if (init_comp > 0)
			{
				min_initiative_idx = i;
				min_initiative = index_turncomp->initiative;
			}
		}
	}

	if (tm->tc_ptr_registry.count > 0)
	{
		tm->active_tc_idx = min_initiative_idx;
		TurnComponent *active_tc = tm->tc_ptr_registry.data[tm->active_tc_idx];
		turncomponent_start_turn(active_tc);
	}
	turnmanager_print_turn_queue(tm);
}

void turnmanager_print_turn_queue(TurnManager *tm)
{
	const TurnCountComponent *tcc = ecs_get(
			tm->world, 
			tm->turn_counter, 
			TurnCountComponent
		);
	printf("Turn %d Queue:\n", tcc->count);
	for (int i = 0; i < tm->tc_ptr_registry.count; i++)
	{
		TurnComponent *tc = tm->tc_ptr_registry.data[i];
		int initiative = tc->initiative;
		printf("Entity:\t%d | %p\ti:%d\n", (uint32_t)tc->entity_id, tc, initiative);
	}
	printf("TurnQueue End \n\n");
}

void turncounter_create(TurnManager *tm, ecs_world_t *world)
{
	tm->turn_counter = ecs_new_id(world);
	ecs_set(world, tm->turn_counter, TurnCountComponent, { .count = 0 });
	ecs_add(world, tm->turn_counter, TurnComponent);
	TurnComponent *tc = ecs_get_mut(world, tm->turn_counter, TurnComponent);
	turncomponent_initialize(tc, tm, tm->turn_counter, world, 0);
}
