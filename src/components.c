#include "components.h"

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(GridPosition);
ECS_COMPONENT_DECLARE(GridVelocity);
ECS_COMPONENT_DECLARE(Glyph);
ECS_COMPONENT_DECLARE(CameraComponent);
ECS_COMPONENT_DECLARE(TurnComponent);
ECS_COMPONENT_DECLARE(TAG_TCEnable);
ECS_COMPONENT_DECLARE(TurnCountComponent);
ECS_COMPONENT_DECLARE(CollisionComponent);
ECS_COMPONENT_DECLARE(CollisionLayer);
ECS_COMPONENT_DECLARE(CollisionMask);
ECS_COMPONENT_DECLARE(PathComponent);
ECS_COMPONENT_DECLARE(NPCTarget);
ECS_COMPONENT_DECLARE(TAG_Player);

ECS_ENTITY_DECLARE(TAG_TurnActive);
ECS_ENTITY_DECLARE(TAG_TurnIdle);

void create_components(ecs_world_t *world)
{
	ECS_COMPONENT_DEFINE(world, Position);
	ECS_COMPONENT_DEFINE(world, Velocity);
	ECS_COMPONENT_DEFINE(world, GridPosition);
	ECS_COMPONENT_DEFINE(world, GridVelocity);
	ECS_COMPONENT_DEFINE(world, Glyph);
	ECS_COMPONENT_DEFINE(world, CameraComponent);
    ECS_COMPONENT_DEFINE(world, TurnComponent);
 	ECS_COMPONENT_DEFINE(world, TAG_TCEnable);
	ECS_COMPONENT_DEFINE(world, TurnCountComponent);
    ECS_COMPONENT_DEFINE(world, CollisionComponent);
    ECS_COMPONENT_DEFINE(world, CollisionLayer);
    ECS_COMPONENT_DEFINE(world, CollisionMask);
    ECS_COMPONENT_DEFINE(world, PathComponent);
    ECS_COMPONENT_DEFINE(world, NPCTarget);
    ECS_COMPONENT_DEFINE(world, TAG_Player);

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
            { ecs_id(CameraComponent) }
        }
    }); 
    ecs_iter_t it = ecs_query_iter(world, query_camera_move);
	while (ecs_query_next(&it))
	{
		CameraComponent *cc = ecs_field(&it, CameraComponent, 1);
		float lerp_factor = 0.1f;

		for (int i = 0; i < it.count; i++)
		{
			const Position *target_pos = ecs_get_mut(
					world, 
					cc[i].target_entity, 
					Position
				);
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
            { ecs_id(GridPosition) },
            { ecs_id(GridVelocity) },
            { ecs_id(Position) },
			{ ecs_id(TAG_TCEnable) },
            { ecs_id(CollisionComponent) },
            { ecs_id(TurnComponent) }
        }
    }); 
    ecs_iter_t it = ecs_query_iter(world, query_grid_move);
	while (ecs_query_next(&it))
	{
		GridPosition *gp = ecs_field(&it, GridPosition, 1);
		GridVelocity *gv = ecs_field(&it, GridVelocity, 2);
		Position *p = ecs_field(&it, Position, 3);
        CollisionComponent *cc = ecs_field(&it, CollisionComponent, 5);
        TurnComponent *tc = ecs_field(&it, TurnComponent, 6);

		for (int i = 0; i < it.count; i++)
		{
			if (gv[i].x == 0 && gv[i].y == 0) { continue; }
            
            Grid *grid = gp[i].grid;
            CollisionData *c_d = cc[i].c_d;
			if (grid) 
			{ 
                int test_x = gp[i].x + gv[i].x;
                int test_y = gp[i].y + gv[i].y;
                int move_test_result = grid_test_place(
                        grid, 
                        c_d->coll_bits, 
                        test_x, 
                        test_y
                    );
                int test_bounds = grid_test_outofbounds(grid, test_x, test_y);
                if (move_test_result != 0 || test_bounds !=0) 
                {
                    continue; 
                }

                grid_move_from(grid, c_d->coll_bits, gp[i].x, gp[i].y);

                gp[i].x += gv[i].x;
                gp[i].y += gv[i].y;
                
                grid_move_to(grid, c_d->coll_bits, gp[i].x, gp[i].y);

				p[i].x = gp[i].x * grid->tile_width;
				p[i].y = gp[i].y * grid->tile_height;
			}
            else
            {
                log_warn("Grid not found.");
            }

            turnmanager_end_turn(tc[i].tc_d->turn_manager, 50);

			int x = gp[i].x;
			int y = gp[i].y;
			int px = p[i].x;
			int py = p[i].y;
			//printf("GP { %d, %d }\n", x, y);
			//printf("P { %d, %d }\n", px, py);
			gv[i].x = 0;
			gv[i].y = 0;
            ecs_remove(it.world, it.entities[i], GridVelocity);
		}
	}
}

void handler_player_input(ecs_world_t *world)
{
    ecs_query_t *query_player_input = ecs_query(world, {
        .filter.terms = {
			{ ecs_id(TurnComponent) },
            { ecs_id(TAG_Player) },
			{ TAG_TurnActive }
        }
    }); 
    ecs_iter_t it = ecs_query_iter(world, query_player_input);
	while (ecs_query_next(&it))
	{
		TurnComponent *tc = ecs_field(&it, TurnComponent, 1);

		int mult = 1;
		for (int i = 0; i < it.count; i++)
		{
            int g_x = 0;
            int g_y = 0;

			TurnManager *tm = tc[i].tc_d->turn_manager;

			int moved = 0;
			if (IsKeyDown(KEY_LEFT_SHIFT))
			{
				mult = 2;
			}
			if (IsKeyPressed(KEY_KP_7) || IsKeyPressed(KEY_KP_8) || IsKeyPressed(KEY_KP_9))
			{
				g_y -= 1 * mult; // UP
				moved = 1;
			}
			if (IsKeyPressed(KEY_KP_1) || IsKeyPressed(KEY_KP_2) || IsKeyPressed(KEY_KP_3))
			{
				g_y += 1 * mult; // DOWN
				moved = 1;
			}
			if (IsKeyPressed(KEY_KP_1) || IsKeyPressed(KEY_KP_4) || IsKeyPressed(KEY_KP_7))
			{
				g_x -= 1 * mult; // LEFT
				moved = 1;
			}
			if (IsKeyPressed(KEY_KP_3) || IsKeyPressed(KEY_KP_6) || IsKeyPressed(KEY_KP_9))
			{
				g_x += 1 * mult; // RIGHT
				moved = 1;
			}
			if (IsKeyPressed(KEY_KP_5))
			{
				moved = 1;
			}

			if (moved == 1)
			{
                /* Add GridVelocity */ // Might replace with a force component?
                ecs_set(it.world, it.entities[i], GridVelocity, { .x = g_x, .y = g_y });
                turnmanager_end_turn(tm, 100);
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
			TurnManager *tm = tc[i].tc_d->turn_manager;
			tcntc[i].count += 1;
			//printf("TurnCount:\t%d\n", tcntc[i].count);
			turnmanager_end_turn(tm, 100);
			//turnmanager_print_turn_queue(tm);
		}
	}
}

void handler_npc_pathfinding(ecs_world_t *world)
{
    ecs_query_t *query_npc_pathfinding = ecs_query(world, {
        .filter.terms = {
            { ecs_id(GridPosition) },
            { ecs_id(GridVelocity) },
            { ecs_id(PathComponent) },
            { ecs_id(NPCTarget) },
            { ecs_id(TurnComponent) },
            { ecs_id(CollisionComponent) },
            { TAG_TurnActive }
        }
    });
    ecs_iter_t it = ecs_query_iter(world, query_npc_pathfinding);
    while (ecs_query_next(&it))
    {
        GridPosition *gp = ecs_field(&it, GridPosition, 1);
        GridVelocity *gv = ecs_field(&it, GridVelocity, 2);
        PathComponent *pc = ecs_field(&it, PathComponent, 3);
        NPCTarget *npct = ecs_field(&it, NPCTarget, 4);
        TurnComponent *tc = ecs_field(&it, TurnComponent, 5);
        CollisionComponent *cc = ecs_field(&it, CollisionComponent, 6);
        

        for (int i = 0; i < it.count; i++)
        {
            const GridPosition *target_gp = ecs_get(world, npct->target, GridPosition);
            TurnManager *tm = tc[i].tc_d->turn_manager;
            if (target_gp == NULL) 
            {
                turnmanager_end_turn(tm, 100);
                continue; 
            }

            CVecInt *path_idx = pathmap_find_path(
                    pc[i].pm,
                    gp[i].x,
                    gp[i].y,
                    target_gp->x,
                    target_gp->y,
                    cc[i].c_d->coll_bits
                );


            if (path_idx == NULL) 
            { 
                turnmanager_end_turn(tm, 100);
                continue; 
            }
            int next_cell_idx = path_idx->data[0];
            cvec_int_free(path_idx);
            int next_x, next_y;
            grid_i2c(gp[i].grid, next_cell_idx, &next_x, &next_y);
            
            // TODO add GridVelocity component, and the velocity to go
        }
    }
}
