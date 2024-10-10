#include "state_gameplayloop.h"

#include <stdio.h>

/* --- Utilities --- */
#include <raylib.h>
#include <raygui.h>
#include <flecs.h>
#include "log.h"

/* --- Dependencies --- */
#include "statemachine.h"
#include "states.h"
#include "main.h"
#include "ecs_components.h"
#include "ecs_gameplay_handlers.h"
#include "serialization.h"
#include "entities.h"
#include "glyph.h"

/* --- Constants --- */
static const char *STATE_NAME = "STATE_GAMEPLAYLOOP\0";

/* --- Variables --- */
const char *TEST_JSON_FILE = "C:\\Projects\\C_Projects\\WastelandAdventures\\data\\test.json";
const char *ENTITY_JSON_FILE = "C:\\Projects\\C_Projects\\WastelandAdventures\\data\\entities.json";
ecs_world_t *gameplay_world;
ecs_entity_t gameplay_camera2d;
Tileset gameplay_tileset;

static int is_gameplayloop_initialized = 0;

/* --- TESTING --- */
ecs_entity_t player;
Glyph test_glyph;
ecs_ref_t *camera;

/* --- SM Function Prototypes --- */
static void enter_state(void);
static void update(double delta);
static void physics_update(double delta);
static void handle_ui(double delta);
static void draw(double delta);
static void exit_state(void);

/* --- Function Prototypes --- */
static void initialize();
static int create_gameplay_world();
static int free_gameplay_world();

void state_gameplayloop_register(SM_Machine *sm, SM_State *state)
{
    state->state_enter = enter_state;
    state->state_update = update;
    state->state_physics_update = physics_update;
    state->state_handle_ui = handle_ui;
    state->state_draw = draw;
    state->state_exit = exit_state;

    sm_register_state(sm, state, STATE_NAME);
}

static void initialize()
{
    is_gameplayloop_initialized = 1;
    create_gameplay_world();

    tileset_initialize(&gameplay_tileset, 
                       "./assets/RDE_8x8.png",
                       8, 8,
                       20, 20,
                       WHITE);
    glyph_initialize(&test_glyph, &gameplay_tileset, 8, 8, WHITE);


    camera = create_camera2d(gameplay_world);
    set_active_camera2d(gameplay_world, camera);

    entity_table_initialize();
    load_entity_definitions_from_file(TEST_JSON_FILE);

    player = create_entity_from_table(gameplay_world, "Player");
    if (player < 10) {
        print_entity_error(player);
    } else {
        const Position2D *p = ecs_get(gameplay_world, player, Position2D);
        log_info("Player: %llu, Position{ %.1f, %.1f }", player, p->x, p->y);
    }
}

static void enter_state(void)
{
    log_info("\n\nGAMEPLAYLOOP_ENTER\n\n");

    if (!is_gameplayloop_initialized) {
        initialize();
    }

}

static void update(double delta)
{

}

static void physics_update(double delta)
{

}

static void handle_ui(double delta)
{

}

static void draw(double delta)
{
    glyph_draw(&test_glyph, 5, 5);
}

static void exit_state(void)
{

}

static int create_gameplay_world()
{
    gameplay_world = ecs_init();
    if (gameplay_world == NULL) {
        return GAMEPLAYWORLD_CREATION_FAILED;
    }
    ecs_define_components(gameplay_world);
    create_gameplay_queries(gameplay_world);

    return GAMEPLAYWORLD_CREATION_SUCCEEDED;
}

static int free_gameplay_world()
{
    if (gameplay_world == NULL) {
        return GAMEPLAYWORLD_FREE_FAILED;
    }
    
    free_gameplay_queries(gameplay_world);
    /* TODO free gameplay_world components */

    ecs_fini(gameplay_world);
    gameplay_world = NULL;
    
    return GAMEPLAYWORLD_FREE_SUCCEEDED;
}
