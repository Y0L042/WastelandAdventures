#ifndef TWEEN_H
#define TWEEN_H

//#define tween_property(_tween_variable_ref, _tween_final_value, _tween_duration, _tween_var_type) \
	

typedef struct Tween {
	
} Tween;

void tween_property(/* variable ref, final value, duration float */);
void tween_callback(/* Callback*/);
void tween_interval(/*Time float */);
void tween_method(/* Callback, input value start, input value end, duration float */);

#endif // TWEEN_H
