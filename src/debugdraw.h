#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <raylib.h>

typedef enum DebugDrawMode {
	DBG_DRAW,
	DBG_UI
} DebugDrawMode;

typedef struct DebugLine {
	int x1, y1, x2, y2;
	Color color;
	double time;
	DebugDrawMode mode;
	double _time_created;
} DebugLine;

void debugdraw_add_line(
		int x1, int y1, int x2, int y2,
		Color color, double time, DebugDrawMode mode
	);

void debugdraw_draw_debugline(double delta);
void debugdraw_ui_debugline(double delta);

void debugdraw_draw(double delta);
void debugdraw_handle_ui(double delta);

void debugdraw_init();

#endif //DEBUGDRAW_H
