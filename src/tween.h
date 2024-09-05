#ifndef TWEEN_H
#define TWEEN_H

#include "flecs.h"

typedef enum TweenValueType {
	TWEEN_VALUE_INT,
	TWEEN_VALUE_FLOAT,
	TWEEN_VALUE_DOUBLE
} TweenValueType;

typedef struct Tween {
	ecs_entity_t tween_ent;
	ecs_world_t *world;
	int initialized;
	double total_time;
	double passed_time;
	int current_loop;
	int running;
	int valid;
} Tween;

typedef struct TweenComponent {
	Tween *tween_ptr;
} TweenComponent;

typedef struct TweenPropertyComponent {
	void *source;
	double target;
	TweenValueType value_type;
	double timeframe;
	double _source_inc; /* _source_inc is used to track small incs when source is int */
	double _init_source; /* initial source value */
} TweenPropertyComponent;



void tween_initialize(Tween *tween, ecs_world_t *world);
void tween_add_property(
		Tween *tween, 
		void *source,
		double target,
		TweenValueType value_type,
		double timeframe
	);

void handler_tween_add_property(ecs_world_t *world, double delta);
void handler_tween_remove_property();



#endif // TWEEN_H
