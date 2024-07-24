#include "components.h"

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(GridPosition);
ECS_COMPONENT_DECLARE(GridVelocity);
ECS_COMPONENT_DECLARE(GridComponent);
ECS_COMPONENT_DECLARE(Glyph);
ECS_COMPONENT_DECLARE(CameraComponent);
ECS_COMPONENT_DECLARE(TurnComponent);
ECS_COMPONENT_DECLARE(TurnCountComponent);

ECS_ENTITY_DECLARE(TAG_Player);
ECS_ENTITY_DECLARE(TAG_TurnActive);
ECS_ENTITY_DECLARE(TAG_TurnIdle);

void create_components(ecs_world_t *world)
{
	ECS_COMPONENT_DEFINE(world, Position);
	ECS_COMPONENT_DEFINE(world, Velocity);
	ECS_COMPONENT_DEFINE(world, GridPosition);
	ECS_COMPONENT_DEFINE(world, GridVelocity);
	ECS_COMPONENT_DEFINE(world, GridComponent);
	ECS_COMPONENT_DEFINE(world, Glyph);
	ECS_COMPONENT_DEFINE(world, CameraComponent);
    ECS_COMPONENT_DEFINE(world, TurnComponent);
	ECS_COMPONENT_DEFINE(world, TurnCountComponent);

    ECS_ENTITY_DEFINE(world, TAG_Player);
    ECS_ENTITY_DEFINE(world, TAG_TurnActive);
    ECS_ENTITY_DEFINE(world, TAG_TurnIdle);
}



void create_queries(ecs_world_t *world)
{
    //ecs_query_t query_draw_glyph;
    //query_draw_glyph = ecs_query_init(world, &(ecs_query_desc_t) {
    //    .terms = {
    //        {ecs_id(Glyph) },
    //        {ecs_id(Position) },
    //        { 0 }  
    //    }
    //}); 
}

void handler_glyph_draw(ecs_world_t *world)
{
    ecs_query_t *query_glyph_draw = ecs_query(world, {
        .filter.terms = {
            {ecs_id(Glyph) },
            {ecs_id(Position) }
        }
    }); 
    ecs_iter_t it = ecs_query_iter(world, query_glyph_draw);
    while (ecs_query_next(&it))
    {
        Glyph *g = ecs_field(&it, Glyph, 1);
        Position *p = ecs_field(&it, Position, 2);

        for (int i = 0; i < it.count; i++)
        {
            Glyph glyph = g[i];
            glyph_draw(&glyph, p[i].x, p[i].y, RAYWHITE);
        }
    }
}


void handler_camera_move(ecs_world_t *world)
{
    ecs_query_t *query_camera_move = ecs_query(world, {
        .filter.terms = {
            {ecs_id(CameraComponent) }
        }
    }); 
    ecs_iter_t it = ecs_query_iter(world, query_camera_move);
	while (ecs_query_next(&it))
	{
		CameraComponent *cc = ecs_field(&it, CameraComponent, 1);
		float lerp_factor = 0.1f;

		for (int i = 0; i < it.count; i++)
		{
			const Position *target_pos = ecs_get_mut(world, cc[i].target_entity, Position);
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
}

void handler_grid_move(ecs_world_t *world)
{
    ecs_query_t *query_grid_move = ecs_query(world, {
        .filter.terms = {
			{ ecs_id(GridComponent) },
            { ecs_id(GridPosition) },
            { ecs_id(GridVelocity) },
            { ecs_id(Position) }
        }
    }); 
    ecs_iter_t it = ecs_query_iter(world, query_grid_move);
	while (ecs_query_next(&it))
	{
		GridComponent *gc = ecs_field(&it, GridComponent, 1);
		GridPosition *gp = ecs_field(&it, GridPosition, 2);
		GridVelocity *gv = ecs_field(&it, GridVelocity, 3);
		Position *p = ecs_field(&it, Position, 4);

		for (int i = 0; i < it.count; i++)
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
			//printf("GP { %d, %d }\n", x, y);
			//printf("P { %d, %d }\n", px, py);
			gv[i].x = 0;
			gv[i].y = 0;
		}
	}
}

void handler_player_input(ecs_world_t *world)
{
    ecs_query_t *query_player_input = ecs_query(world, {
        .filter.terms = {
            { ecs_id(GridVelocity) },
			{ ecs_id(TurnComponent) },
			{ TAG_TurnActive }
        }
    }); 
    ecs_iter_t it = ecs_query_iter(world, query_player_input);
	while (ecs_query_next(&it))
	{
		GridVelocity *gv = ecs_field(&it, GridVelocity, 1);
		TurnComponent *tc = ecs_field(&it, TurnComponent, 2);

		int mult = 1;
		for (int i = 0; i < it.count; i++)
		{
			gv[i].x = 0;
			gv[i].y = 0;

			TurnManager *tm = tc[i].turn_manager;

			int moved = 0;
			if (IsKeyDown(KEY_LEFT_SHIFT))
			{
				mult = 2;
			}
			if (IsKeyPressed(KEY_KP_7) || IsKeyPressed(KEY_KP_8) || IsKeyPressed(KEY_KP_9))
			{
				gv[i].y -= 1 * mult; // UP
				moved = 1;
			}
			if (IsKeyPressed(KEY_KP_1) || IsKeyPressed(KEY_KP_2) || IsKeyPressed(KEY_KP_3))
			{
				gv[i].y += 1 * mult; // DOWN
				moved = 1;
			}
			if (IsKeyPressed(KEY_KP_1) || IsKeyPressed(KEY_KP_4) || IsKeyPressed(KEY_KP_7))
			{
				gv[i].x -= 1 * mult; // LEFT
				moved = 1;
			}
			if (IsKeyPressed(KEY_KP_3) || IsKeyPressed(KEY_KP_6) || IsKeyPressed(KEY_KP_9))
			{
				gv[i].x += 1 * mult; // RIGHT
				moved = 1;
			}
			if (IsKeyPressed(KEY_KP_5))
			{
				moved = 1;
			}

			if (moved == 1)
			{
				static int cntr = 0;
				//printf("Ending player turn:\t%d\n", ++cntr);
				turnmanager_next_turn(tm, &tc[i], 50);
				//turnmanager_print_turn_queue(tm);
			}
		}
	}
}

void handler_turncounter_increment(ecs_world_t *world)
{
    ecs_query_t *query_turncounter_increment = ecs_query(world, {
        .filter.terms = {
            { ecs_id(TurnCountComponent) },
			{ ecs_id(TurnComponent) },
			{ TAG_TurnActive }
        }
    }); 
	//log_debug("DEBUG");
    ecs_iter_t it = ecs_query_iter(world, query_turncounter_increment);
	while (ecs_query_next(&it))
	{
		TurnCountComponent *tcntc = ecs_field(&it, TurnCountComponent, 1);
		TurnComponent *tc = ecs_field(&it, TurnComponent, 2);

		for (int i = 0; i < it.count; i++)
		{
			TurnManager *tm = tc[i].turn_manager;
			tcntc[i].count += 1;
			//printf("TurnCount:\t%d\n", tcntc[i].count);
			turnmanager_next_turn(tm, &tc[i], 100);
			//turnmanager_print_turn_queue(tm);
		}
	}
}

