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
	turnmanager_add_turncomponent(tm, tc);
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
    int result = tc_a->initiative - tc_b->initiative;
	if (result == 0)
	{
		double time_diff = tc_a->last_turn_time - tc_b->last_turn_time;
		result = time_diff < 0 ? -1 : 1;
	}
    return result;
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
	cvec_void_init(&tm->tc_data);

	TurnComponentData *tc_d = turncounter_create(tm, world);
    turncomponentdata_start_turn(tc_d);
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
	cvec_void_add_data(&tm->tc_datas, tc_d);

	ecs_set(tm->world, entity, TurnComponent, { .tc_d = tc_d });
	ecs_ref_t *tc_ref = (ecs_ref_t *)malloc(sizeof(ecs_ref_t));
	*tc_ref = ecs_ref_init(tm->world, entity, TurnComponent);
	cvec_void_add_data(&tm->tc_refs, tc_ref);
	tc_d->tc_ref = tc_ref;

	return tc_d;
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
	turnmanager_create_turncomponent(tm, tm->turn_counter);
}
