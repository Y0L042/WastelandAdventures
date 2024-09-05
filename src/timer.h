#ifndef TIMER_H
#define TIMER_H

#include "flecs.h"

typedef struct Timer {
	ecs_world_t *world;
	ecs_entity_t timer_ent;
	double current_time;
	double target_time;
	void (*callback)(void);
	int started;
	int running;
	int stopped;
	int valid;
} Timer;

typedef struct TimerActiveComponent { Timer *timer_ptr; } TimerActiveComponent;

Timer *timer_create(ecs_world_t *world);
void timer_free(Timer *timer);
void timer_start(Timer *timer, double target, void (*callback)(void));

void handler_process_timers(ecs_world_t *world, double delta);

#endif // TIMER_H
