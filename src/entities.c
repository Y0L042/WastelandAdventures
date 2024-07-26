#include "entities.h"

ecs_entity_t g_ent_player;
ecs_entity_t g_ent_camera;

void ent_player_create(
		ecs_entity_t *ent_player,
		ecs_world_t *world, 
		TurnManager *tm, 
		Grid* grid_worldspace,
		Tileset *tileset
	)
{
	*ent_player = ecs_new_id(world);
    ecs_add(world, *ent_player, TAG_Player);
    ecs_set(world, *ent_player, Position, { .x = 0, .y = 0 });
    ecs_set(world, *ent_player, Velocity, { .x = 0, .y = 0 });
    ecs_set(world, *ent_player, GridPosition, { .x = 0, .y = 0 });
    ecs_set(world, *ent_player, GridVelocity, { .x = 0, .y = 0 });
    ecs_set(world, *ent_player, GridComponent, { 
			.grid = grid_worldspace, 
			.tile = NULL 
		});
    ecs_set(world, *ent_player, Glyph, { 
			.source_tile_x = 0, 
			.source_tile_y = 4,
			.tileset = tileset 
		}); 
	turnmanager_create_turncomponent(tm, *ent_player);
}




void ent_camera_create(
		ecs_entity_t *ent_camera,
		ecs_world_t *world,
		ecs_entity_t *ent_target
	)
{
	*ent_camera = ecs_new_id(world);
	
	Camera2D new_camera = { 
			(Vector2){ (g_SCREEN_WIDTH / 2), (g_SCREEN_HEIGHT / 2) },
			(Vector2){ 0.0f, 0.0f },
			0,
			1
		};
	ecs_set(world, *ent_camera, CameraComponent, { 
			.camera = new_camera,
			.target_entity = *ent_target,
			.previous_target_entity = 0	
		});
}
