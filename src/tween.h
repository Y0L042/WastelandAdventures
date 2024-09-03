#ifndef TWEEN_H
#define TWEEN_H

#include "flecs.h"

typedef ecs_entity_t Tween;	

typedef enum TweenValueType {
	TWEEN_VALUE_INT,
	TWEEN_VALUE_FLOAT,
	TWEEN_VALUE_DOUBLE
} TweenValueType;

typedef struct TweenComponent {
	double total_time;
	double passed_time;
	int current_loop;
	int started;
	int running;
	int valid;
} TweenComponent;

typedef struct TweenPropertyComponent {
	void *source;
	double target;
	enum TweenValueType value_type;
	double timeframe;
	double _source_inc; /* _source_inc is used to track small incs when source is int */
} TweenPropertyComponent;



Tween tween_create(ecs_world_t *world);
void tween_add_property(
		ecs_world_t *world,
		Tween tween, 
		void *source,
		double target,
		TweenValueType value_type,
		double timeframe
	);

void handler_tween_add_property(ecs_world_t *world, double delta);
void handler_tween_remove_property();



#endif // TWEEN_H
