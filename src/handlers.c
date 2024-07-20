#include "handlers.h"

ecs_entity_t glyph_draw_sys;
ecs_entity_t grid_move_sys;
ecs_entity_t player_input_sys;
ecs_entity_t camera_move_sys;

void create_handlers(ecs_world_t* world)
{
    glyph_draw_sys = ecs_system_init(world, &(ecs_system_desc_t) {
        .query.filter.terms = {
            { .id = ecs_id(Glyph) },
            { .id = ecs_id(Position) },
            { 0 }  // Zero-terminate the array
        },
        .callback = handler_glyph_draw
    });

    grid_move_sys = ecs_system_init(world, &(ecs_system_desc_t) {
        .query.filter.terms = {
            //{ .id = ecs_id(TAG_TurnActive) },
            { .id = ecs_id(GridComponent) },
            { .id = ecs_id(GridPosition) },
            { .id = ecs_id(GridVelocity) },
            { .id = ecs_id(Position) },
            { 0 }  // Zero-terminate the array
        },
        .callback = handler_grid_move
    });

    player_input_sys = ecs_system_init(world, &(ecs_system_desc_t) {
        .query.filter.terms = {
            { .id = ecs_id(TAG_TurnActive) },
            { .id = ecs_id(GridVelocity) },
            { 0 }  // Zero-terminate the array
        },
        .callback = handler_player_input
    });

    camera_move_sys = ecs_system_init(world, &(ecs_system_desc_t) {
        .query.filter.terms = {
            { .id = ecs_id(CameraComponent) },
            { 0 }  // Zero-terminate the array
        },
        .callback = handler_camera_move 
    });
}

void handler_grid_move(ecs_iter_t *it)
{
	GridComponent *gc = ecs_field(it, GridComponent, 1);
	GridPosition *gp = ecs_field(it, GridPosition, 2);
	GridVelocity *gv = ecs_field(it, GridVelocity, 3);
	Position *p = ecs_field(it, Position, 4);

	for (int i = 0; i < it->count; i++)
	{
		if (gv[i].x == 0 && gv[i].y == 0) { continue; }

		gp[i].x += gv[i].x;
		gp[i].y += gv[i].y;

		if (gc[i].grid) 
		{
            p[i].x = gp[i].x * gc[i].grid->tile_width;
            p[i].y = gp[i].y * gc[i].grid->tile_height;
        }

		int x = gp[i].x;
		int y = gp[i].y;
		int px = p[i].x;
		int py = p[i].y;
		printf("GP { %d, %d }\n", x, y);
		printf("P { %d, %d }\n", px, py);
		gv[i].x = 0;
		gv[i].y = 0;
	}
}

void handler_glyph_draw(ecs_iter_t* it)
{
	Glyph *g = ecs_field(it, Glyph, 1);
	Position *p = ecs_field(it, Position, 2);

	for (int i = 0; i < it->count; i++)
	{
		Glyph glyph = g[i];
		glyph_draw(&glyph, p[i].x, p[i].y, RAYWHITE);
	}
}

void handler_player_input(ecs_iter_t* it)
{
	GridVelocity *gv = ecs_field(it, GridVelocity, 1);
	int mult = 1;
	for (int i = 0; i < it->count; i++)
	{
		gv[i].x = 0;
		gv[i].y = 0;

		if (IsKeyDown(KEY_LEFT_SHIFT))
		{
			mult = 2;
		}
		if (IsKeyPressed(KEY_KP_7) || IsKeyPressed(KEY_KP_8) || IsKeyPressed(KEY_KP_9))
		{
			gv[i].y -= 1 * mult; // UP
		}
		if (IsKeyPressed(KEY_KP_1) || IsKeyPressed(KEY_KP_2) || IsKeyPressed(KEY_KP_3))
		{
			gv[i].y += 1 * mult; // DOWN
		}
		if (IsKeyPressed(KEY_KP_1) || IsKeyPressed(KEY_KP_4) || IsKeyPressed(KEY_KP_7))
		{
			gv[i].x -= 1 * mult; // LEFT
		}
		if (IsKeyPressed(KEY_KP_3) || IsKeyPressed(KEY_KP_6) || IsKeyPressed(KEY_KP_9))
		{
			gv[i].x += 1 * mult; // RIGHT
		}
	}
}

void handler_camera_move(ecs_iter_t *it)
{
	CameraComponent *cc = ecs_field(it, CameraComponent, 1);
	
	float lerp_factor = 0.1f;

	for (int i = 0; i < it->count; i++)
	{
		const Position *target_pos = ecs_get(it->world, cc[i].target_entity, Position);
		if (target_pos)
		{
			Vector2 current_target = cc[i].camera.target;
			Vector2 desired_target = (Vector2){ target_pos->x, target_pos->y };
			cc[i].camera.target = Vector2Lerp(current_target, desired_target, lerp_factor);
		}
		else
		{
			printf("No target_pos found\n");
		}
	}
}
