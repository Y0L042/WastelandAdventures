#include "entities.h"

ecs_entity_t g_ent_player;
ecs_entity_t g_ent_camera;
ecs_entity_t g_ent_dog;

const int ENT_PLAYER_COLL_LAYER = 0b0011;
const int ENT_PLAYER_COLL_MASK = 0b0001;
const int ENT_DOG_COLL_LAYER = 0b0010;
const int ENT_DOG_COLL_MASK = 0b0011;
const int ENT_WALL_PERM_COLL_LAYER = 0b0001;
const int ENT_WALL_PERM_COLL_MASK = 0b0000;




void ent_camera_create(
		ecs_entity_t *ent_camera,
		ecs_world_t *world,
		ecs_entity_t entity_target
	)
{
	*ent_camera = ecs_new(world);
	
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

void ent_player_create(
		ecs_entity_t *ent_player,
		ecs_world_t *world, 
		TurnManager *tm, 
		Grid* grid,
		Tileset *tileset,
        int grid_x, int grid_y
	)
{
//    log_debug("ent_player_create START");
	
    *ent_player = ecs_new(world);
    ecs_add(world, *ent_player, TAG_Player);
    int world_x, world_y;
    grid_pos_to_world_pos(grid, grid_x, grid_y, &world_x, &world_y);
    ecs_set(world, *ent_player, Position, { .x = world_x, .y = world_y });
	grid_create_gridposition(
			grid_x, grid_y,
			grid, 
			*ent_player,
			ENT_PLAYER_COLL_LAYER,
			ENT_PLAYER_COLL_MASK
		);

    ecs_set(world, *ent_player, Glyph, { 
			.source_tile_x = 0, 
			.source_tile_y = 4,
			.tileset = tileset,
			.color = RAYWHITE
		}); 
	turnmanager_create_turncomponent(tm, *ent_player);
    
//    log_debug("ent_player_create END");
}

void ent_dog_create(
		ecs_entity_t *ent_dog,
		ecs_world_t *world, 
		TurnManager *tm, 
		Grid* grid,
		Tileset *tileset,
		ecs_entity_t entity_target,
		int grid_x, int grid_y
	)
{
	*ent_dog = ecs_new(world);
    int world_x, world_y;
    grid_pos_to_world_pos(grid, grid_x, grid_y, &world_x, &world_y);
	ecs_set(world, *ent_dog, Position, { .x = world_x, .y = world_y });
	ecs_set(world, *ent_dog, Glyph, {
			.source_tile_x = 13,
			.source_tile_y = 6,
			.tileset = tileset,
			.color = BROWN
		});
	grid_create_gridposition(
			grid_x, grid_y,
			grid, 
			*ent_dog,
			ENT_DOG_COLL_LAYER,
			ENT_DOG_COLL_MASK
		);
	turnmanager_create_turncomponent(tm, *ent_dog);
	//turnmanager_disable_tc(tm, *ent_dog);
	cmp_add_PathComponent(world, *ent_dog, grid);
    ecs_set(world, *ent_dog, NPCTarget, { .target = entity_target });
}

void ent_wall_perm_create(
        ecs_entity_t *ent_wall_perm,
        ecs_world_t *world,
        Grid *grid,
        Tileset *tileset,
        int grid_x, int grid_y
    )
{
   *ent_wall_perm = ecs_new(world);
   int world_x, world_y;
   grid_pos_to_world_pos(grid, grid_x, grid_y, &world_x, &world_y);
   ecs_set(world, *ent_wall_perm, Position, { .x = world_x, .y = world_y });
   grid_create_gridposition(
			grid_x, grid_y,
			grid, 
			*ent_wall_perm,
			ENT_WALL_PERM_COLL_LAYER,
			ENT_WALL_PERM_COLL_MASK
		);
   ecs_set(world, *ent_wall_perm, Glyph, {
           .source_tile_x = 11,
           .source_tile_y = 13,
           .tileset = tileset,
		   .color = LIGHTGRAY
        });
}
