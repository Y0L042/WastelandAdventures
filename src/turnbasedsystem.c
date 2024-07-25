#include "turnbasedsystem.h"
#include <math.h>

void turnmanager_initialize(TurnManager *tm, ecs_world_t *world)
{
	tm->world = world;
	cvec_void_init(&tm->tc_refs);
	turncounter_create(tm, world);
    TurnComponent *tc = ecs_get_mut(world, tm->turn_counter, TurnComponent);
    turnmanager_start_turn(tc);
}

void turnmanager_add_turncomponent(TurnManager *tm, ecs_entity_t entity)
{
	ecs_ref_t *tc_ref = (ecs_ref_t *)malloc(sizeof(ecs_ref_t));
	*tc_ref = ecs_ref_init(tm->world, entity, TurnComponent);
	cvec_void_add_item(&tm->tc_refs, tc_ref);
}

void turnmanager_remove_turncomponent(TurnManager *tm, ecs_ref_t* tc_ref_ptr)
{
	for (int i = 0; i < tm->tc_refs.count; i++)
	{
		if (tc_ref_ptr == tm->tc_refs.data[i])
		{
			cvec_void_remove_idx_noshrink(&tm->tc_refs, i);
			return;
		}
	}
}

void turnmanager_next_turn(TurnManager *tm, TurnComponent *tc, int inc)
{
	turncomponent_end_turn(tc, inc);
		
    //for (int i = 1; i < tm->tc_refs.count; i++) 
    //{
    //    TurnComponent *key = tm->tc_refs.data[i];
    //    int j = i - 1;
    //    while (
	//		j >= 0 
	//		&& turncomponent_compare_initiatives(
	//			tm->tc_refs.data[j], key
	//		) > 0
	//	) 
    //    {
    //        tm->tc_refs.data[j + 1] = tm->tc_refs.data[j];
    //        j = j - 1;
    //    }
    //    tm->tc_refs.data[j + 1] = key;
    //}

	int min_initiative_idx = 0; 
	int min_initiative = -1;
	for (int i = 0; i < tm->tc_refs.count; i++)
	{
		log_debug("TurnComponent DEBUG");
		/*
		*	Loop thru each turncomponent. If that component has a lower initiative,
		*	or older (smaller) last_turn_time, set its idx as min_initiative_idx.
		*	It will be the next component to be TurnActive.
		*/
		TurnComponent *index_turncomp = (TurnComponent *)(tm->tc_refs.data[i]);
		if (index_turncomp->initiative <= min_initiative || min_initiative < 0)
		{
			int init_comp = turncomponent_compare_initiatives(
					index_turncomp,
					tm->tc_refs.data[min_initiative_idx]
				);
			if (init_comp > 0)
			{
				min_initiative_idx = i;
				min_initiative = index_turncomp->initiative;
			}
		}
	}

	if (tm->tc_refs.count > 0)
	{
		tm->active_tc_idx = min_initiative_idx;
		TurnComponent *active_tc = tm->tc_refs.data[tm->active_tc_idx];
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
	for (int i = 0; i < tm->tc_refs.count; i++)
	{
		TurnComponent *tc = tm->tc_refs.data[i];
		int initiative = tc->initiative;
		printf("Entity:\t%d | %p\ti:%d\n", (uint32_t)tc->entity_id, tc, initiative);
	}
	printf("TurnQueue End \n\n");
}


void turnmanager_inc_tc_initiative(TurnManager *tm, ecs_ref_t *tc_ref, int inc)
{
	TurnComponent *tc = ecs_ref_get(tm->world, tc_ref, TurnComponent);		
	tc->initiative += inc;
}

int  turnmanager_compare_tc(TurnManager *tm, ecs_ref_t *tc_ref_a, ecs_ref_t *tc_ref_b)
{
	TurnComponent *tc_A = ecs_ref_get(tm->world, tc_ref_a, TurnComponent);		
	TurnComponent *tc_B = ecs_ref_get(tm->world, tc_ref_b, TurnComponent);		

	int result = tc_A->initiative - tc_B->initiative;
	if (result == 0)
	{
		double time_diff = tc_A->last_turn_time - tc_B->last_turn_time;
		result = time_diff < 0 ? -1 : 1;
	}

	return result; 
}

void turnmanager_change_tc_state(TurnManager *tm, ecs_ref_t *tc_ref, enum TurnState new_state)

{
	TurnComponent *tc = ecs_ref_get(tm->world, tc_ref, TurnComponent);
	tc->current_turn_state = new_state;
}

void turnmanager_start_turn(TurnManager *tm, ecs_ref_t *tc_ref)
{
	TurnComponent *tc = ecs_ref_get(tm->world, tc_ref, TurnComponent);
	ecs_remove(tm->world, tc_ref->entity, TAG_TurnIdle);
	ecs_add(tm->world, tc_ref->entity, TAG_TurnActive);
	turnmanager_change_tc_state(tm, tc_ref, TURNSTATE_ACTIVE);
}

void turnmanager_end_turn(TurnManager *tm, ecs_ref_t *tc_ref, int inc)
{
	TurnComponent *tc = ecs_ref_get(tm->world, tc_ref, TurnComponent);
	ecs_remove(tm->world, tc_ref->entity, TAG_TurnActive);
	ecs_add(tm->world, tc_ref->entity, TAG_TurnIdle);
	tc-last_turn_time = GetTime();
	turnmanager_inc_tc_initiative(tm, tc_ref, inc);
	turnmanager_change_tc_state(tm, tc_ref, TURNSTATE_IDLE);
}

void turncounter_create(TurnManager *tm, ecs_world_t *world)
{
	tm->turn_counter = ecs_new_id(world);
	ecs_set(world, tm->turn_counter, TurnCountComponent, { .count = 0 });
	ecs_add(world, tm->turn_counter, TurnComponent);
	TurnComponent *tc = ecs_get_mut(world, tm->turn_counter, TurnComponent);
	turncomponent_initialize(tc, tm, tm->turn_counter, world, 0);
}
