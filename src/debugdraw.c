#include "debugdraw.h"
#include <raygui.h>
#include "dray.h"
#include "maths.h"

static int debugdraw_initialized = 0;
static DRay debugline_draw_queue;
static DRay debugline_ui_queue;

void debugdraw_add_line(
		int x1, int y1, int x2, int y2,
		Color color, double time, DebugDrawMode mode
	)
{
	debugdraw_init();
	double _current_time = GetTime();
	DebugLine s = { x1, y1, x2, y2, color, time, mode, _current_time };
	dray_add_value(&debugline_draw_queue, s, DebugLine);	
}

void debugdraw_draw_debugline(double delta)
{
	for (int i = 0; i < debugline_draw_queue.count; i++)
	{
		DebugLine s = dray_get_value(&debugline_draw_queue, i, DebugLine);
		DrawLine(s.x1, s.y1, s.x2, s.y2, s.color);

		double _current_time = GetTime();
		if (maths_cmpd(s.time, 0.0) || (s.time < (_current_time - s._time_created)))
		{
			dray_remove_idx(&debugline_draw_queue, i--);
		}
	}
}
void debugdraw_ui_debugline(double delta)
{
	for (int i = 0; i < debugline_ui_queue.count; i++)
	{
		DebugLine s = dray_get_value(&debugline_ui_queue, i, DebugLine);
		DrawLine(s.x1, s.y1, s.x2, s.y2, s.color);

		double _current_time = GetTime();
		if (maths_cmpd(s.time, 0.0) || (s.time < (_current_time - s._time_created)))
		{
			dray_remove_idx(&debugline_ui_queue, i--);
		}
	}
}
void debugdraw_draw(double delta)
{
	debugdraw_draw_debugline(delta);
}

void debugdraw_handle_ui(double delta)
{
	debugdraw_ui_debugline(delta);
}

void debugdraw_init()
{
	if (debugdraw_initialized) { return; }
	debugdraw_initialized = 1;

	dray_init_values(&debugline_draw_queue, DebugLine);			
	dray_init_values(&debugline_ui_queue, DebugLine);			
}
