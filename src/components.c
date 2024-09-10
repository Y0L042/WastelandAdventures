#include "components.h"

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(GridPosition);
ECS_COMPONENT_DECLARE(GridVelocity);
ECS_COMPONENT_DECLARE(Glyph);
ECS_COMPONENT_DECLARE(GlyphFade);
ECS_COMPONENT_DECLARE(LeaveGlyphGhost);
ECS_COMPONENT_DECLARE(GhostWhenMoving);
ECS_COMPONENT_DECLARE(CameraComponent);
ECS_COMPONENT_DECLARE(TurnComponent);
ECS_COMPONENT_DECLARE(TAG_TCEnable);
ECS_COMPONENT_DECLARE(TurnCountComponent);
ECS_COMPONENT_DECLARE(TurnEnter);
ECS_COMPONENT_DECLARE(TurnProcess);
ECS_COMPONENT_DECLARE(TurnExit);
ECS_COMPONENT_DECLARE(PathComponent);
ECS_COMPONENT_DECLARE(NPCTarget);
ECS_COMPONENT_DECLARE(TAG_Player);
ECS_COMPONENT_DECLARE(HealthComponent);
ECS_COMPONENT_DECLARE(TAG_EntEnabled);
ECS_COMPONENT_DECLARE(TAG_EntDisabled);
ECS_COMPONENT_DECLARE(TAG_TurnActive);
ECS_COMPONENT_DECLARE(TAG_TurnIdle);
ECS_COMPONENT_DECLARE(GridArea);
ECS_COMPONENT_DECLARE(VisionArea);
ECS_COMPONENT_DECLARE(TriggerArea);
ECS_COMPONENT_DECLARE(DamageComponent);
ECS_COMPONENT_DECLARE(HurtComponent);
ECS_COMPONENT_DECLARE(DeathComponent);
ECS_COMPONENT_DECLARE(EntDeletionQueued);
ECS_COMPONENT_DECLARE(TweenComponent);
ECS_COMPONENT_DECLARE(TweenPropertyComponent);
ECS_COMPONENT_DECLARE(TimerActiveComponent);
ECS_COMPONENT_DECLARE(AIComponent);

void create_components(ecs_world_t *world)
{
	ECS_COMPONENT_DEFINE(world, Position);
	ECS_COMPONENT_DEFINE(world, Velocity);
	ECS_COMPONENT_DEFINE(world, GridPosition);
	ECS_COMPONENT_DEFINE(world, GridVelocity);
	ECS_COMPONENT_DEFINE(world, Glyph);
	ECS_COMPONENT_DEFINE(world, GlyphFade);
	ECS_COMPONENT_DEFINE(world, LeaveGlyphGhost);
	ECS_COMPONENT_DEFINE(world, GhostWhenMoving);
	ECS_COMPONENT_DEFINE(world, CameraComponent);
    ECS_COMPONENT_DEFINE(world, TurnComponent);
 	ECS_COMPONENT_DEFINE(world, TAG_TCEnable);
	ECS_COMPONENT_DEFINE(world, TurnCountComponent);
	ECS_COMPONENT_DEFINE(world, TurnEnter);
	ECS_COMPONENT_DEFINE(world, TurnProcess);
	ECS_COMPONENT_DEFINE(world, TurnExit);
    ECS_COMPONENT_DEFINE(world, NPCTarget);
    ECS_COMPONENT_DEFINE(world, TAG_Player);
	ECS_COMPONENT_DEFINE(world, PathComponent);
	ECS_COMPONENT_DEFINE(world, HealthComponent);
	ECS_COMPONENT_DEFINE(world, TAG_EntEnabled);
	ECS_COMPONENT_DEFINE(world, TAG_EntDisabled);
    ECS_COMPONENT_DEFINE(world, TAG_TurnActive);
    ECS_COMPONENT_DEFINE(world, TAG_TurnIdle);
	ECS_COMPONENT_DEFINE(world, GridArea);
	ECS_COMPONENT_DEFINE(world, VisionArea);
	ECS_COMPONENT_DEFINE(world, TriggerArea);
	ECS_COMPONENT_DEFINE(world, DamageComponent);
	ECS_COMPONENT_DEFINE(world, HurtComponent);
	ECS_COMPONENT_DEFINE(world, DeathComponent);
	ECS_COMPONENT_DEFINE(world, EntDeletionQueued);
	ECS_COMPONENT_DEFINE(world, TweenComponent);
	ECS_COMPONENT_DEFINE(world, TweenPropertyComponent);
	ECS_COMPONENT_DEFINE(world, TimerActiveComponent);
	ECS_COMPONENT_DEFINE(world, AIComponent);
}



static ecs_query_t *query_glyph_draw = NULL;
static ecs_query_t *query_glyph_ghowst_spawn = NULL;
static ecs_query_t *query_glyph_fade = NULL;
static ecs_query_t *query_camera_move = NULL;
static ecs_query_t *query_grid_move = NULL;
static ecs_query_t *query_player_input = NULL;
static ecs_query_t *query_pathfinding = NULL;
static ecs_query_t *query_turncounter_increment = NULL;
static ecs_query_t *query_draw_health_ui = NULL;
static ecs_query_t *query_process_traps = NULL;
static ecs_query_t *query_process_triggerareas = NULL;
static ecs_query_t *query_process_visionareas = NULL;
static ecs_query_t *query_process_hurt = NULL;
static ecs_query_t *query_process_death = NULL;
static ecs_query_t *query_process_entity_delection = NULL;
static ecs_query_t *query_process_ai = NULL;

void create_queries(ecs_world_t *world)
{
    query_glyph_draw = ecs_query(world, {
        .terms = {
            { ecs_id(Glyph) },
            { ecs_id(Position) }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_glyph_ghowst_spawn = ecs_query(world, {
        .terms = {
            { ecs_id(LeaveGlyphGhost) },
            { ecs_id(Glyph) },
            { ecs_id(Position) }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_glyph_fade = ecs_query(world, {
        .terms = {
            { ecs_id(Glyph) },
			{ ecs_id(GlyphFade) }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_camera_move = ecs_query(world, {
        .terms = {
            { ecs_id(CameraComponent) }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_grid_move = ecs_query(world, {
        .terms = {
            { ecs_id(GridPosition) },
            { ecs_id(GridVelocity) },
            { ecs_id(Position) },
			{ ecs_id(TAG_TCEnable) },
            { ecs_id(TurnComponent) },
			{ ecs_id(GhostWhenMoving), .oper = EcsOptional }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_player_input = ecs_query(world, {
        .terms = {
			{ ecs_id(TurnComponent) },
            { ecs_id(TAG_Player) },
			{ ecs_id(TAG_TurnActive) }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_pathfinding = ecs_query(world, {
        .terms = {
			{ ecs_id(TurnComponent) },
			{ ecs_id(TAG_TurnActive) },
			{ ecs_id(PathComponent) },
			{ ecs_id(GridPosition) },
			{ ecs_id(NPCTarget) },
			{ ecs_id(GridVelocity), .oper = EcsNot }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_turncounter_increment = ecs_query(world, {
        .terms = {
            { ecs_id(TurnCountComponent) },
			{ ecs_id(TurnComponent) },
			{ ecs_id(TAG_TurnActive) }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_draw_health_ui = ecs_query(world, {
        .terms = {
			{ ecs_id(TAG_Player) },
            { ecs_id(HealthComponent) }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_process_traps = ecs_query(world, {
        .terms = {
			{ ecs_id(TAG_Player) },
            { ecs_id(HealthComponent) }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_process_triggerareas = ecs_query(world, {
        .terms = {
            { ecs_id(TriggerArea) },
			{ ecs_id(GridPosition) },
			{ ecs_id(TurnComponent) },
			{ ecs_id(TAG_TurnActive) }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_process_visionareas = ecs_query(world, {
        .terms = {
            { ecs_id(VisionArea) },
			{ ecs_id(GridPosition) },
			{ ecs_id(TurnComponent) },
			{ ecs_id(TAG_TurnActive) }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_process_hurt = ecs_query(world, {
        .terms = {
            { ecs_id(HealthComponent) },
			{ ecs_id(HurtComponent) }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_process_death = ecs_query(world, {
        .terms = {
            { ecs_id(HealthComponent) },
            { ecs_id(DeathComponent) }
        },
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
    }); 

    query_process_entity_delection = ecs_query(world, {
        .terms = {
            { ecs_id(EntDeletionQueued) }
        },
    }); 

	query_process_ai = ecs_query(world, {
		.terms = {
			{ ecs_id(AIComponent) },
			{ ecs_id(TAG_TurnActive) }
		},
		.cache_kind = EcsQueryCacheAuto,
		.flags = EcsQueryMatchEmptyTables
	});
}

void free_queries(ecs_world_t *world)
{
	ecs_query_fini(query_glyph_draw);
	ecs_query_fini(query_glyph_ghowst_spawn);
	ecs_query_fini(query_glyph_fade);
	ecs_query_fini(query_camera_move);
	ecs_query_fini(query_grid_move);
	ecs_query_fini(query_player_input);
	ecs_query_fini(query_pathfinding);
	ecs_query_fini(query_turncounter_increment);
	ecs_query_fini(query_draw_health_ui);
	ecs_query_fini(query_process_traps);
	ecs_query_fini(query_process_triggerareas);
	ecs_query_fini(query_process_visionareas);
	ecs_query_fini(query_process_hurt);
	ecs_query_fini(query_process_death);
	ecs_query_fini(query_process_entity_delection);
	ecs_query_fini(query_process_ai);
}



void handler_glyph_draw(ecs_world_t *world)
{
    ecs_iter_t it = ecs_query_iter(world, query_glyph_draw);
    while (ecs_query_next(&it))
    {
        Glyph *g = ecs_field(&it, Glyph, 0);
        Position *p = ecs_field(&it, Position, 1);

        for (int i = 0; i < it.count; i++)
        {
            Glyph glyph = g[i];
            glyph_draw(&glyph, p[i].x, p[i].y);
        }
    }
}



/* Forward Declaration */
void ent_glyph_ghost_create(
		ecs_world_t *world,
		Glyph *glyph,
		double time,
		int world_x, int world_y
	);
void handler_glyph_ghost_spawn(ecs_world_t *world)
{
    ecs_iter_t it = ecs_query_iter(world, query_glyph_ghowst_spawn);
    while (ecs_query_next(&it))
    {
        LeaveGlyphGhost *gg = ecs_field(&it, LeaveGlyphGhost, 0);
        Glyph *g = ecs_field(&it, Glyph, 1);
        Position *p = ecs_field(&it, Position, 2);

        for (int i = 0; i < it.count; i++)
        {
			//log_info("Ghost created");
			ent_glyph_ghost_create(it.world, &g[i], gg[i].fade_time, gg[i].x, gg[i].y);
			ecs_remove(it.world, it.entities[i], LeaveGlyphGhost);
        }
    }
}



void handler_glyph_fade(ecs_world_t *world, double delta)
{
    ecs_iter_t it = ecs_query_iter(world, query_glyph_fade);
    while (ecs_query_next(&it))
    {
        Glyph *g = ecs_field(&it, Glyph, 0);
		GlyphFade *gf = ecs_field(&it, GlyphFade, 1);

        for (int i = 0; i < it.count; i++)
        {
			double fade_perc = gf[i].time_left / gf[i].initial_time;
			//g[i].color.a = ((double)g[i]._init_color.a) * fade_perc;
			g[i].color.a = (215 / 2.25) * fade_perc;
			gf[i].time_left -= delta;

			if ((fade_perc * 100.0) < 0.25) 
			{
				ecs_delete(it.world, it.entities[i]);
			}
        }
    }
}



void handler_camera_move(ecs_world_t *world)
{
    ecs_iter_t it = ecs_query_iter(world, query_camera_move);
	while (ecs_query_next(&it))
	{
		CameraComponent *cc = ecs_field(&it, CameraComponent, 0);
		float lerp_factor = 0.1f;

		for (int i = 0; i < it.count; i++)
		{
			if (!ecs_is_alive(it.world, cc[i].target_entity)) { continue; }

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
				//cc[i].camera.target.x = trunc(cc[i].camera.target.x);
				//cc[i].camera.target.y = trunc(cc[i].camera.target.y);
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
    ecs_iter_t it = ecs_query_iter(world, query_grid_move);
	while (ecs_query_next(&it))
	{
		GridPosition *gp = ecs_field(&it, GridPosition, 0);
		GridVelocity *gv = ecs_field(&it, GridVelocity, 1);
		Position *p = ecs_field(&it, Position, 2);
        TurnComponent *tc = ecs_field(&it, TurnComponent, 4);

		for (int i = 0; i < it.count; i++)
		{
			int vel_x = gv[i].x;
			int vel_y = gv[i].y;

			if (gv[i].x == 0 && gv[i].y == 0) 
			{
				turnmanager_end_turn(tc[i].tc_d->turn_manager, 50);
				ecs_remove(it.world, it.entities[i], GridVelocity);
				continue; 
			}
           	int old_x, old_y; 
            Grid *grid = gp[i].grid;
			if (grid) 
			{
				int coll_mask = 0;
				int coll_layer = 0;
				gridposition_move(
						&gp[i], 
						gp[i].x + gv[i].x, 
						gp[i].y + gv[i].y, 
						0						
					);
               	old_x = p[i].x;
				old_y = p[i].y;
				p[i].x = gp[i].x * grid->tile_width;
				p[i].y = gp[i].y * grid->tile_height;
			} else
            {
                log_warn("Grid not found.");
            }

			TurnComponentData *tc_d = tc[i].tc_d;
			log_info("%s is moving", tc_d->alias);
            turnmanager_end_turn(tc[i].tc_d->turn_manager, 50);

			gv[i].x = 0;
			gv[i].y = 0;
            ecs_remove(it.world, it.entities[i], GridVelocity);

			if (ecs_field_is_set(&it, 5) && !(vel_x ==0 && vel_y == 0)) 
			{
				ecs_set(it.world, it.entities[i], LeaveGlyphGhost, { 
						.fade_time = 1.2, .x = old_x, .y = old_y });
			}
		}
	}
}



void handler_player_input(ecs_world_t *world)
{
    ecs_iter_t it = ecs_query_iter(world, query_player_input);
	while (ecs_query_next(&it))
	{
		TurnComponent *tc = ecs_field(&it, TurnComponent, 0);

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



void handler_pathfinding(ecs_world_t *world)
{
	ecs_iter_t it = ecs_query_iter(world, query_pathfinding);
	while(ecs_query_next(&it))
	{
		PathComponent *pc = ecs_field(&it, PathComponent, 2);
		GridPosition *gp = ecs_field(&it, GridPosition, 3);
		NPCTarget *target = ecs_field(&it, NPCTarget, 4);

		for (int i = 0; i < it.count; i++)
		{
			Grid *grid = gp[i].grid;
			if (!ecs_is_alive(world, target->target)) { continue; }
			const GridPosition *gp_target = ecs_get(world, target->target, GridPosition);
			if (gp_target == NULL) 
			{ 
				//ecs_remove(it.world, it.entities[i], PathComponent);
				ecs_set(it.world, it.entities[i], GridVelocity, { .x = 0, .y = 0 });
				continue; 
			}

			DRay path;
			dray_init_values(&path, Vector2);
			find_path(
					&path,
					grid, 
					gp[i].x, 
					gp[i].y, 
					gp_target->x, 
					gp_target->y, 
					gp[i].coll_mask
				);
			if (path.count == 0) 
			{ 
				//ecs_remove(it.world, it.entities[i], PathComponent);
				ecs_set(it.world, it.entities[i], GridVelocity, { .x = 0, .y = 0 });
				continue; 
			}
			
		//	log_info("PATHFIND TARGET  : { %d, %d }", gp_target->x, gp_target->y);
			Vector2 next_pos = dray_pop_value(&path, Vector2);
		//	log_info("PATHFIND NEXT_POS: { %.f, %.f }", next_pos.x, next_pos.y);
			Vector2 vel = { next_pos.x - gp[i].x, next_pos.y - gp[i].y };
		//	log_info("POS: { %d, %d }", gp[i].x, gp[i].y);
		//	log_info("VEL: { %.f, %.f }", vel.x, vel.y);
			ecs_set(it.world, it.entities[i], GridVelocity, { .x = vel.x, .y = vel.y });
			
			for (int i = 0; i < path.count; i++)
			{
				Vector2 p = dray_get_value(&path, i, Vector2);
		//		log_info("PATH { %d, %d }", p.x, p.y);
			}
		}
	}
}



void handler_turncounter_increment(ecs_world_t *world)
{
	//log_debug("DEBUG");
    ecs_iter_t it = ecs_query_iter(world, query_turncounter_increment);
	while (ecs_query_next(&it))
	{
		TurnCountComponent *tcntc = ecs_field(&it, TurnCountComponent, 0);
		TurnComponent *tc = ecs_field(&it, TurnComponent, 1);

		for (int i = 0; i < it.count; i++)
		{
			TurnManager *tm = tc[i].tc_d->turn_manager;
			tcntc[i].count += 1;
			//printf("TurnCount:\t%d\n", tcntc[i].count);
			turnmanager_end_turn(tm, 100);
			// turnmanager_print_turn_queue(tm);
		}
	}
}



void handler_draw_health_ui(ecs_world_t *world)
{

    ecs_iter_t it = ecs_query_iter(world, query_draw_health_ui);
	while (ecs_query_next(&it))
	{
		HealthComponent *hc = ecs_field(&it, HealthComponent, 1);

		for (int i = 0; i < it.count; i++)
		{
			char healthText[50];
			sprintf(healthText, "Health: %d", hc[i].health);
			DrawText(healthText, 20, 40, 22, RED);
		}
	}
}



void handler_process_traps(ecs_world_t *world)
{

    ecs_iter_t it = ecs_query_iter(world, query_process_traps);
	while (ecs_query_next(&it))
	{
		HealthComponent *hc = ecs_field(&it, HealthComponent, 1);

		for (int i = 0; i < it.count; i++)
		{
			char healthText[50];
			sprintf(healthText, "Health: %d", hc[i].health);
			DrawText(healthText, 20, 40, 22, RED);
		}
	}
}



void handler_process_triggerareas(ecs_world_t *world)
{

    ecs_iter_t it = ecs_query_iter(world, query_process_triggerareas);
	while (ecs_query_next(&it))
	{
		TriggerArea *ta = ecs_field(&it, TriggerArea, 0);
		GridPosition *gpt = ecs_field(&it, GridPosition, 1);
		TurnComponent *tc = ecs_field(&it, TurnComponent, 2);

		for (int i = 0; i < it.count; i++)
		{
			Grid *grid = gpt[i].grid;
			DRay entities;
			dray_init_values(&entities, ecs_entity_t);
			int success = grid_get_entities_in_area(&gpt[i], &ta[i], &entities) == 0;
			if (success)
			{
				// log_info("TriggerArea: enemy found!");
			}

			if (success && (ta[i].callback != NULL))
			{
				(*ta[i].callback)(&ta[i], it.world, &entities);
			}

			TurnManager *tm = tc[i].tc_d->turn_manager;
			turnmanager_end_turn(tm, 100);
		}
	}
}



void handler_process_visionareas(ecs_world_t *world)
{
    ecs_iter_t it = ecs_query_iter(world, query_process_visionareas);
	while (ecs_query_next(&it))
	{
		VisionArea *va = ecs_field(&it, VisionArea, 0);
		GridPosition *gpt = ecs_field(&it, GridPosition, 1);
		TurnComponent *tc = ecs_field(&it, TurnComponent, 2);

		for (int i = 0; i < it.count; i++)
		{
			Grid *grid = gpt[i].grid;

			DRay entities;
			dray_init_values(&entities, ecs_entity_t);
			int success = grid_get_entities_in_area(&gpt[i], &va[i], &entities) == 0;
			if (success)
			{
				// log_info("VisionArea: enemy spotted!");
			}

			if (success && (va[i].callback != NULL))
			{
				(*va[i].callback)(&va[i], it.world, &entities);
			}

			//dray_free(entities);

			TurnManager *tm = tc[i].tc_d->turn_manager;
			turnmanager_end_turn(tm, 75);
		}
	}
}



void handler_process_hurt(ecs_world_t *world)
{

    ecs_iter_t it = ecs_query_iter(world, query_process_hurt);
	while (ecs_query_next(&it))
	{
		HealthComponent *healthc = ecs_field(&it, HealthComponent, 0);
		HurtComponent *hurtc = ecs_field(&it, HurtComponent, 1);

		for (int i = 0; i < it.count; i++)
		{
			healthc[i].health -= hurtc[i].hurt;
			ecs_remove(it.world, it.entities[i], HurtComponent);

			if (healthc[i].health < 0)
			{
				ecs_add(it.world, it.entities[i], DeathComponent);
			}

			if (healthc[i].callback_onhurt != NULL)
			{
				(*healthc[i].callback_onhurt)(it.world, it.entities[i], hurtc[i].hurt);
			}
		}
	}
}



void handler_process_death(ecs_world_t *world)
{

    ecs_iter_t it = ecs_query_iter(world, query_process_death);
	while (ecs_query_next(&it))
	{
		HealthComponent *healthc = ecs_field(&it, HealthComponent, 0);

		for (int i = 0; i < it.count; i++)
		{
			if (healthc[i].callback_ondeath != NULL)
			{
				(*healthc[i].callback_ondeath)(it.world, it.entities[i], healthc[i].health);
			}
			
			ecs_add(it.world, it.entities[i], EntDeletionQueued);
		}
	}
}



void handler_process_entity_deletion(ecs_world_t *world)
{

    ecs_iter_t it = ecs_query_iter(world, query_process_entity_delection);
	while (ecs_query_next(&it))
	{

		for (int i = 0; i < it.count; i++)
		{
			ecs_delete(it.world, it.entities[i]);
		}
	}
}

void handler_process_ai(ecs_world_t *world)
{
    ecs_iter_t it = ecs_query_iter(world, query_process_ai);
    while (ecs_query_next(&it))
    {
        AIComponent *ai = ecs_field(&it, AIComponent, 0);

        for (int i = 0; i < it.count; i++)
        {
			(*ai[i].ai_main)(&ai[i]);
        }
    }
}

