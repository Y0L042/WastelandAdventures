#include "turnbasedsystem.h"
#include <math.h>

void turncomponentdata_initialize(
		TurnComponentData *tc_d, 
		TurnManager *tm, 
		ecs_entity_t entity_id, 
		ecs_world_t *world, 
		int init_initiative
	)
{
	tc_d->initiative = init_initiative;
    tc_d->world = world;
    tc_d->entity_id = entity_id;
	tc_d->turn_manager = tm;
	tc_d->current_turn_state = TURNSTATE_IDLE;
}

void turncomponentdata_free(TurnComponentData *tc_d)
{
	free(tc_d);
}

void turncomponentdata_inc_initiative(TurnComponentData *tc_d, int inc)
{
	tc_d->initiative += inc;
}

int turncomponentdata_compare_initiatives(TurnComponentData *tc_d_a, TurnComponentData *tc_d_b)
{
    int result = tc_d_a->initiative - tc_d_b->initiative;
	if (result == 0)
	{
		double time_diff = tc_d_a->last_turn_time - tc_d_b->last_turn_time;
		result = time_diff < 0 ? -1 : 1;
	}
    return result; // neg means a is smaller, pos means b is smaller
}

void turncomponentdata_change_state(TurnComponentData *tc_d, enum TurnState new_state)
{
    tc_d->current_turn_state = new_state;
}

void turncomponentdata_start_turn(TurnComponentData *tc_d)
{
    ecs_remove(tc_d->world, tc_d->entity_id, TAG_TurnIdle);
    ecs_add(tc_d->world, tc_d->entity_id, TAG_TurnActive);
    turncomponentdata_change_state(tc_d, TURNSTATE_ACTIVE);
}

void turncomponentdata_end_turn(TurnComponentData *tc_d, int inc)
{
    ecs_remove(tc_d->world, tc_d->entity_id, TAG_TurnActive);
	ecs_add(tc_d->world, tc_d->entity_id, TAG_TurnIdle);
	tc_d->last_turn_time = GetTime();
	turncomponentdata_inc_initiative(tc_d, inc);
	turncomponentdata_change_state(tc_d, TURNSTATE_IDLE);
}



void turnmanager_initialize(TurnManager *tm, ecs_world_t *world)
{
	tm->world = world;

	cvec_void_init(&tm->tc_refs);
	cvec_void_init(&tm->tc_datas);
	tm->tracked_tc_count = 0;

	turncounter_create(tm, world);
}

/*
*	First, it creates the TurnComponentData, that will store the TC data in an accessible,
*	mutable struct. It adds a ptr of this to TM.
*
*	Then it creates a TurnComponent for the entity, that will hold a pointer to the
*	TC data struct, for access within the ecs framework.
*	For good measure, it adds an ecs_ref of this component to TM and the TC data struct.
*/
TurnComponentData* turnmanager_create_turncomponent(TurnManager *tm, ecs_entity_t entity)
{
	TurnComponentData *tc_d = (TurnComponentData *)malloc(sizeof(TurnComponentData));
	turncomponentdata_initialize(tc_d, tm, entity, tm->world, 0); 

	ecs_set(tm->world, entity, TurnComponent, { .tc_d = tc_d });
	ecs_ref_t *tc_ref = (ecs_ref_t *)malloc(sizeof(ecs_ref_t));
	*tc_ref = ecs_ref_init(tm->world, entity, TurnComponent);
	tc_d->tc_ref = tc_ref;

	cvec_void_add_item(&tm->tc_datas, tc_d);
	cvec_void_add_item(&tm->tc_refs, tc_ref);
	tm->tracked_tc_count += 1;

	return tc_d;
}


//void turnmanager_remove_turncomponent(TurnManager *tm, TurnComponent *tc)
//{
//	for (int i = 0; i < tm->tc_ptr_registry.count; i++)
//	{
//		if (tc == tm->tc_ptr_registry.data[i])
//		{
//			cvec_void_remove_idx_noshrink(&tm->tc_ptr_registry, i);
//			return;
//		}
//	}
//}

void turnmanager_end_turn(TurnManager *tm, int inc)
{
	/*
	*	Find the current active turncomponent. end its turn. 
	*	Sort the turncomponents based on initiative.
	*	Then activate the next turncomponent.
	*/
	ecs_ref_t *active_tc_ref = (ecs_ref_t *)tm->tc_refs.data[tm->active_tc_idx];
	const TurnComponent *active_tc = ecs_ref_get(tm->world, active_tc_ref, TurnComponent);
	TurnComponentData *active_tc_d = active_tc->tc_d;
	turncomponentdata_end_turn(active_tc_d, inc);
		
	/*
	*	Loop thru each turncomponent. If that component has a lower initiative,
	*	or older (smaller) last_turn_time, set its idx as min_initiative_idx.
	*	It will be the next component to be TurnActive.
	*/
	int min_initiative_idx = 0; 
	int min_initiative = -1;
	for (int i = 0; i < tm->tracked_tc_count; i++)
	{
		TurnComponentData *tc_d_i = (TurnComponentData *)(tm->tc_datas.data[i]);
		if (tc_d_i->initiative <= min_initiative || min_initiative < 0)
		{
			int init_comp = turncomponentdata_compare_initiatives(
					tc_d_i,
					tm->tc_datas.data[min_initiative_idx]
				);
			log_info("i: %d", i);
			log_info("init_comp: %d", init_comp);
			if (init_comp < 0)
			{
				min_initiative_idx = i;
				TurnComponentData *tc_d_current_min = cvec_void_get_item(tm->tc_datas, i, TurnComponentData);
				min_initiative = tc_d_current_min->initiative;
			}
		}
	}

	if (tm->tracked_tc_count > 0)
	{
		tm->active_tc_idx = min_initiative_idx;
		TurnComponentData *next_tc_d = tm->tc_datas.data[tm->active_tc_idx];
		turncomponentdata_start_turn(next_tc_d);
	}
	turnmanager_print_turn_queue(tm);
}

void turnmanager_print_turn_queue(TurnManager *tm)
{
	const TurnCountComponent *tcc = ecs_ref_get(
			tm->world, 
			tm->turn_counter_ref, 
			TurnCountComponent
		);
	printf("Turn %d Queue:\n", tcc->count);
	for (int i = 0; i < tm->tracked_tc_count; i++)
	{
		TurnComponentData *tc_d = tm->tc_datas.data[i];
		int initiative = tc_d->initiative;
		printf("Entity:\t%d | %p\ti:%d\n", (uint32_t)tc_d->entity_id, tc_d, initiative);
	}
	printf("TurnQueue End \n\n");
}

void turncounter_create(TurnManager *tm, ecs_world_t *world)
{
	tm->turn_counter = ecs_new_id(world);
	ecs_set(world, tm->turn_counter, TurnCountComponent, { .count = 0 });
	
	ecs_ref_t *tcntr_ref = (ecs_ref_t *)malloc(sizeof(ecs_ref_t));
	*tcntr_ref = ecs_ref_init(tm->world, tm->turn_counter, TurnCountComponent);
	tm->turn_counter_ref = tcntr_ref;

	TurnComponentData *tc_d = turnmanager_create_turncomponent(tm, tm->turn_counter);
    turncomponentdata_start_turn(tc_d);
}
