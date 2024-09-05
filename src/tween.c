#include "tween.h"
#include "components.h"
#include "maths.h"

#define TWEEN_INF_LOOP_CUTTOFF 10000



void tween_initialize(Tween *tween, ecs_world_t *world)
{
	tween->tween_ent = ecs_new(world);
	tween->world = world;
	tween->initialized = 1;
	tween->total_time = 0.0;
	tween->passed_time = 0.0;
	tween->current_loop = 0;
	tween->running = 0;
	tween->valid = 0;

	ecs_set(world, tween->tween_ent, TweenComponent, {
			.tween_ptr = tween
		});
}

void tween_add_property(
		Tween *tween, 
		void *source,
		double target,
		TweenValueType value_type,
		double timeframe
	)
{
	tween->running = 1;
	tween->valid = 1;
	tween->passed_time = 0.0;

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

	ecs_set(tween->world, tween->tween_ent, TweenPropertyComponent, {
			.source = source,
			.target = target,
			.value_type = value_type,
			.timeframe = timeframe,
			._source_inc = _src_inc,
			._init_source = _src_inc
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
			Tween *tween = tc[i].tween_ptr;

			/* Update tween properties */
			tween->current_loop++;
			tween->passed_time += delta;
			tween->total_time += delta;

			/* Increment source_inc */
			tpc[i]._source_inc += ((tpc[i].target - tpc[i]._init_source) / tpc[i].timeframe) * delta;

			/* Tween cutoff conditions */
			if ( 	(tween->current_loop > TWEEN_INF_LOOP_CUTTOFF)
					|| (tween->passed_time >= tpc[i].timeframe)
					|| (maths_cmpd(tpc[i]._source_inc, tpc[i].target)) )
			{
				/* Force set source to target value on tween finish */
				switch (tpc[i].value_type) {
					case TWEEN_VALUE_INT:
						*(int *)tpc[i].source  = (int)tpc[i].target;
						break;
					case TWEEN_VALUE_FLOAT:
						*(float *)tpc[i].source  = (float)tpc[i].target;
						break;
					case TWEEN_VALUE_DOUBLE:
						*(double *)tpc[i].source  = tpc[i].target;
						break;
					default:
						continue;
						break;
				}

				log_info("SRCi: %.5f", tpc[i]._source_inc);
				log_info("SRC : %.5f", *(double *)tpc[i].source);
				log_info("TRG : %.5f", tpc[i].target);
				log_info("TIM : %.1f", tween->passed_time);

				ecs_remove(it.world, it.entities[i], TweenPropertyComponent);
				tween->running = 0;
				continue;
			}

			/* Update source */
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

