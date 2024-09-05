#include "timer.h"
#include "components.h"
#include "maths.h"

Timer *timer_create(ecs_world_t *world)
{
	Timer *timer = (Timer *)malloc(sizeof(Timer));
	timer->world = world;
	timer->timer_ent = ecs_new(world);
	timer->valid = 1;
	timer->current_time = 0.0;
	timer->target_time = 0.0;
	
	return timer;
}

void timer_free(Timer *timer)
{
	if (timer->world != NULL)
	{
		if (ecs_is_alive(timer->world, timer->timer_ent))
		{
			ecs_delete(timer->world, timer->timer_ent);
		}
	}

	free(timer);
	timer = NULL;
}

void timer_start(Timer *timer, double target, void (*callback)(void))
{
	if (!timer->valid) { return; }	
	timer->target_time = target;
	timer->callback = callback;
	ecs_set(timer->world, timer->timer_ent, TimerActiveComponent, {
			.timer_ptr = timer
		});
}


void handler_process_timers(ecs_world_t *world, double delta)
{
    ecs_query_t *query_process_timers = ecs_query(world, {
        .terms = {
            { ecs_id(TimerActiveComponent) }
        },
    }); 

    ecs_iter_t it = ecs_query_iter(world, query_process_timers);
	while (ecs_query_next(&it))
	{
		TimerActiveComponent *tac = ecs_field(&it, TimerActiveComponent, 0);

		for (int i = 0; i < it.count; i++)
		{
			Timer *timer = tac[i].timer_ptr;
			if (timer->valid != 1) { continue; }

			timer->current_time += delta;

			if ((maths_cmpd(timer->current_time, timer->target_time)) 
					|| (timer->current_time > timer->target_time))
			{
				timer->running = 0;
				timer->stopped = 1;
				timer->valid = 0;
				if (timer->callback != NULL) 
				{
					(timer->callback)();
				} 
				timer_free(timer);
				ecs_delete(it.world, it.entities[i]);
				continue;
			}
		}
	}
	ecs_query_fini(query_process_timers);
}

