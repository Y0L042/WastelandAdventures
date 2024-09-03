#include "tween.h"
#include "components.h"

#define TWEEN_INF_LOOP_CUTTOFF 10000

typedef ecs_entity_t Tween;	


Tween tween_create(ecs_world_t *world)
{
	Tween tween = ecs_new(world);

	ecs_set(world, tween, TweenComponent, {
			.total_time = 0.0,
			.passed_time = 0.0,
			.current_loop = 0,
			.started = 0,
			.running = 0,
			.valid = 0
			});

	return tween;
}

void tween_add_property(
		ecs_world_t *world,
		Tween tween, 
		void *source,
		double target,
		TweenValueType value_type,
		double timeframe
	)
{
	TweenComponent *tc = ecs_get_mut(world, tween, TweenComponent);
	tc->started = 1;
	tc->running = 1;
	tc->valid = 1;
	tc->passed_time = 0.0;

	double _src_inc = 0.0;
	switch (value_type) {
		case TWEEN_VALUE_INT:
			int i_src = *(int *)source;
			_src_inc = (double)i_src;
			break;
		case TWEEN_VALUE_FLOAT:
			float f_src = *(float *)source;
			_src_inc = (double)f_src;
			break;
		case TWEEN_VALUE_DOUBLE:
			_src_inc = *(double *)source;
			break;
		default:
			_src_inc = 0.0;
			break;
	}

	ecs_set(world, tween, TweenPropertyComponent, {
			.source = source,
			.target = target,
			.value_type = value_type,
			.timeframe = timeframe,
			._source_inc = _src_inc
		});
}

void handler_tween_add_property(ecs_world_t *world, double delta)
{
    ecs_query_t *query_tween_add_property = ecs_query(world, {
        .terms = {
			{ ecs_id(TweenComponent) },
			{ ecs_id(TweenPropertyComponent) }
        }
    }); 

    ecs_iter_t it = ecs_query_iter(world, query_tween_add_property);
	while (ecs_query_next(&it))
	{
		TweenComponent *tc = ecs_field(&it, TweenComponent, 0);
		TweenPropertyComponent *tpc = ecs_field(&it, TweenPropertyComponent, 1);

		for (int i = 0; i < it.count; i++)
		{
			if (
					(tc[i].current_loop > TWEEN_INF_LOOP_CUTTOFF)
					|| (tc[i].passed_time > tpc[i].timeframe)
				)
			{
				continue;
			}

			tc[i].current_loop++;
			tc[i].passed_time += delta;
			tc[i].total_time += delta;

			tpc[i]._source_inc += (tpc[i].target / tpc[i].timeframe) * delta;

			switch (tpc[i].value_type) {
				case TWEEN_VALUE_INT:
					*(int *)tpc[i].source  = (int)round(tpc[i]._source_inc);
					break;
				case TWEEN_VALUE_FLOAT:
					*(float *)tpc[i].source  = (float)tpc[i]._source_inc;
					break;
				case TWEEN_VALUE_DOUBLE:
					*(double *)tpc[i].source  = tpc[i]._source_inc;
					break;
				default:
					continue;
					break;
			}
		}
	}
	ecs_query_fini(query_tween_add_property);
}

