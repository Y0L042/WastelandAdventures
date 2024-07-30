#include "entities.h"

ecs_entity_t g_ent_player;
ecs_entity_t g_ent_camera;
ecs_entity_t g_ent_dog;

void ent_player_create(
		ecs_entity_t *ent_player,
		ecs_world_t *world, 
		TurnManager *tm, 
		Grid* grid_worldspace,
		Tileset *tileset
	)
{
//    log_debug("ent_player_create START");
	
    *ent_player = ecs_new_id(world);
    ecs_add(world, *ent_player, TAG_Player);
    ecs_set(world, *ent_player, Position, { .x = 0, .y = 0 });
    ecs_set(world, *ent_player, Velocity, { .x = 0, .y = 0 });
    ecs_set(world, *ent_player, GridPosition, { .x = 0, .y = 0 });
    ecs_set(world, *ent_player, GridVelocity, { .x = 0, .y = 0 });
    ecs_set(world, *ent_player, Glyph, { 
			.source_tile_x = 0, 
			.source_tile_y = 4,
			.tileset = tileset 
		}); 
    grid_create_gridcomponent(grid_worldspace, *ent_player);
	turnmanager_create_turncomponent(tm, *ent_player);
    
//    log_debug("ent_player_create END");
}




void ent_camera_create(
		ecs_entity_t *ent_camera,
		ecs_world_t *world,
		ecs_entity_t entity_target
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
			.target_entity = entity_target,
			.previous_target_entity = 0	
		});
}

void ent_dog_create(
		ecs_entity_t *ent_dog,
		ecs_world_t *world, 
		TurnManager *tm, 
		Grid* grid_worldspace,
		Tileset *tileset,
		ecs_entity_t entity_target
	)
{
	*ent_dog = ecs_new_id(world);
	ecs_set(world, *ent_dog, Position, { .x = 0, .y = 0 });
	ecs_set(world, *ent_dog, Velocity, { .x = 0, .y = 0 });
	ecs_set(world, *ent_dog, GridPosition, { .x = 0, .y = 0 });
	ecs_set(world, *ent_dog, Glyph, {
			.source_tile_x = 0,
			.source_tile_y = 3,
			.tileset = tileset
		});
	turnmanager_create_turncomponent(tm, *ent_dog);
	turnmanager_disable_tc(tm, *ent_dog);
}
