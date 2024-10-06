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
#include "serialization.h"

/* --- Constants --- */
static const char *STATE_NAME = "STATE_GAMEPLAYLOOP\0";

/* --- Variables --- */
const char *TEST_JSON_FILE = "C:\\Projects\\C_Projects\\WastelandAdventures\\data\\test.json";
const char *ENTITY_JSON_FILE = "C:\\Projects\\C_Projects\\WastelandAdventures\\data\\entities.json";
ecs_world_t *gameplay_world;
ecs_entity_t gameplay_camera2d;

static int is_gameplayloop_initialized = 0;

/* --- TESTING --- */
ecs_entity_t player;

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

}

static void enter_state(void)
{
    log_info("\n\nGAMEPLAYLOOP_ENTER\n\n");

    if (!is_gameplayloop_initialized) {
        initialize();
    }

    player = ecs_new(gameplay_world);
    ecs_set(gameplay_world, player, Position2D, { .x = 10.0f, .y = 20.0f });
    serialize_entity_to_json_file(gameplay_world, player, ENTITY_JSON_FILE);

    ecs_entity_t dog = deserialize_entity_from_json_file(gameplay_world, TEST_JSON_FILE);
    const Position2D *p = ecs_get(gameplay_world, dog, Position2D);
    log_info("DOG: .id %llu Position{ %.1f, %.1f }\n", dog, p->x, p->y);
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
    /* TODO create gameplay_world queries */

    return GAMEPLAYWORLD_CREATION_SUCCEEDED;
}

static int free_gameplay_world()
{
    if (gameplay_world == NULL) {
        return GAMEPLAYWORLD_FREE_FAILED;
    }
    ecs_fini(gameplay_world);
    gameplay_world = NULL;

    
    /* TODO free gameplay_world components */
    /* TODO free gameplay_world queries */
    return GAMEPLAYWORLD_FREE_SUCCEEDED;
}
