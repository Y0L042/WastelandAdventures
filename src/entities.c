#include "entities.h"

#include "maths.h"

ecs_entity_t g_ent_player;
ecs_entity_t g_ent_camera;
ecs_entity_t g_ent_dog;


#ifndef COLL_LAYERS_DEFINED
#define COLL_LAYERS_DEFINED
	#define COLL_LAYER_EMPTY 		0b00000000
	#define COLL_LAYER_ALL			0b11111111
	#define COLL_LAYER_WORLD 		0b00000001
	#define COLL_LAYER_PLAYER 		0b00000010
	#define COLL_LAYER_FRIENDLIES 	0b00000100
	#define COLL_LAYER_ENEMIES 		0b00001000
	#define COLL_LAYER_TRAPS 		0b00010000
	#define COLL_LAYER_1			0b00100000
	#define COLL_LAYER_2			0b01000000
	#define COLL_LAYER_3			0b10000000
#endif // COLL_LAYERS_DEFINED


const int ENT_PLAYER_COLL_LAYER = 	COLL_LAYER_PLAYER;
const int ENT_PLAYER_COLL_MASK = 	COLL_LAYER_WORLD;

const int ENT_DOG_COLL_LAYER = 	COLL_LAYER_FRIENDLIES;
const int ENT_DOG_COLL_MASK = 	COLL_LAYER_PLAYER 
								| COLL_LAYER_WORLD;

const int ENT_WALL_PERM_COLL_LAYER = 	COLL_LAYER_WORLD;
const int ENT_WALL_PERM_COLL_MASK = 	COLL_LAYER_EMPTY;

const int ENT_FLOORTRAP_BASIC_COLL_LAYER = 	COLL_LAYER_TRAPS;
const int ENT_FLOORTRAP_BASIC_COLL_MASK = 	COLL_LAYER_PLAYER 
											| COLL_LAYER_FRIENDLIES 
											| COLL_LAYER_ENEMIES;



void ent_camera_create(
		ecs_entity_t *ent_camera,
		ecs_world_t *world,
		ecs_entity_t entity_target
	)
{
	*ent_camera = ecs_new(world);
	ecs_add(world, *ent_camera, TAG_EntEnabled);
	
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
	ecs_add(world, *ent_player, TAG_EntEnabled);

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
			.source_tile_x = 27,//0, 
			.source_tile_y = 0,//4,
			.tileset = tileset,
			.color = RAYWHITE
		}); 
	turnmanager_create_turncomponent(tm, *ent_player);

	ecs_set(world, *ent_player, HealthComponent, { .health = 100, ._initial_health = 100 });
    
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
	ecs_add(world, *ent_dog, TAG_EntEnabled);
    int world_x, world_y;
    grid_pos_to_world_pos(grid, grid_x, grid_y, &world_x, &world_y);
	ecs_set(world, *ent_dog, Position, { .x = world_x, .y = world_y });
	ecs_set(world, *ent_dog, Glyph, {
			.source_tile_x = 10,//13,
			.source_tile_y = 20,//6,
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
	ecs_set(world, *ent_dog, GhostWhenMoving, { .fade_time = 5 });
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
   int rand_x = maths_randbetween_int(0, 1);
   ecs_set(world, *ent_wall_perm, Glyph, {
           .source_tile_x = 0 + rand_x,//11,
           .source_tile_y = 2,//13,
           .tileset = tileset,
		   .color = LIGHTGRAY
        });
}

void ent_glyph_ghost_create(
		ecs_world_t *world,
		Glyph *glyph,
		double time,
		int world_x, int world_y
	)
{
	ecs_entity_t ent_glyph_ghost = ecs_new(world);
	ecs_add(world, ent_glyph_ghost, TAG_EntEnabled);
	ecs_set(world, ent_glyph_ghost, Position, { .x = world_x, .y = world_y });
	ecs_set(world, ent_glyph_ghost, Glyph, {
		   .source_tile_x =  glyph->source_tile_x,//11,
		   .source_tile_y =  glyph->source_tile_y,//13,
		   .tileset = glyph->tileset,
		   .color = glyph->color
		});
	ecs_set(world, ent_glyph_ghost, GlyphFade, { 
			.initial_time = time, 
			.time_left = time 
		});
}

void callback_floor_trap_basic_damage(ecs_world_t *world, DRay *entities)
{
	for (int i = 0; i < entities->count; i++)
	{
		ecs_entity_t ent = dray_get_value(entities, i, ecs_entity_t);
		if (ecs_get(world, ent, HealthComponent))
		{
			printf("Entity %d takes damage!\n", ent);
		}
	}
}

void ent_floor_trap_basic_create(
		ecs_entity_t *ent_floortrap_basic,
		ecs_world_t *world,
		TurnManager *tm, 
		Grid *grid,
		Tileset *tileset,
		int grid_x, int grid_y
	)
{
    *ent_floortrap_basic = ecs_new(world);
	ecs_add(world, *ent_floortrap_basic, TAG_EntEnabled);
    int world_x, world_y;
    grid_pos_to_world_pos(grid, grid_x, grid_y, &world_x, &world_y);
    ecs_set(world, *ent_floortrap_basic, Position, { .x = world_x, .y = world_y });
	grid_create_gridposition(
			grid_x, grid_y,
			grid, 
			*ent_floortrap_basic,
			ENT_FLOORTRAP_BASIC_COLL_LAYER,
			ENT_FLOORTRAP_BASIC_COLL_MASK
		);

    ecs_set(world, *ent_floortrap_basic, Glyph, { 
			.source_tile_x = 3, 
			.source_tile_y = 2,
			.tileset = tileset,
			.color = { 255, 255, 255, 100 }
		}); 

	ecs_set(world, *ent_floortrap_basic, TriggerArea, {
			.x = grid_x, .y = grid_y,
			.rad = 0,
			.mode = 's',
			.area_mask = ENT_FLOORTRAP_BASIC_COLL_MASK,
			.callback = callback_floor_trap_basic_damage
		});



	turnmanager_create_turncomponent(tm, *ent_floortrap_basic);
}


