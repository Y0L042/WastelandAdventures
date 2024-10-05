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

/* --- Constants --- */
static const char *STATE_NAME = "STATE_GAMEPLAYLOOP\0";

/* --- Variables --- */
const char *ENTITY_JSON_FILE = "C:\\Projects\\C_Projects\\WastelandAdventures\\data\\test.json";
ecs_world_t *gameplay_world;
ecs_entity_t gameplay_camera2d;

static int is_gameplayloop_initialized = 0;

/* --- TESTING --- */
ecs_entity_t player;
typedef struct Position { int x, y; } Position;
ECS_COMPONENT_DECLARE(Position);

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

    ECS_COMPONENT_DEFINE(gameplay_world, Position);
    ecs_struct(gameplay_world, {
        .entity = ecs_id(Position), // Make sure to use existing id
        .members = {
            { .name = "x", .type = ecs_id(ecs_i32_t) }, // builtin float type
            { .name = "y", .type = ecs_id(ecs_i32_t) }
        }
    });
}

static void enter_state(void)
{
    if (!is_gameplayloop_initialized) {
        initialize();
    }

    player = ecs_new(gameplay_world);
    ecs_set(gameplay_world, player, Position, { .x = 10, .y = 20 });
}

static void update(double delta)
{

}

static void physics_update(double delta)
{
    static int ent_to_json_done = 0;
    if (!ent_to_json_done) {
        ent_to_json_done = 1;
        ecs_entity_to_json_desc_t params = ECS_ENTITY_TO_JSON_INIT;
        char *json_entity;
        json_entity = ecs_entity_to_json(gameplay_world, player, NULL);
        printf("ENTITY_JSON_START\n%s\nENTITY_JSON_END\n", json_entity);
        ecs_os_free(json_entity);
    }

    static int json_to_ent_done = 0;
    if (!json_to_ent_done) {
        json_to_ent_done = 1;
        FILE *file = fopen(ENTITY_JSON_FILE, "r");
        char buffer[1000];
        int i = 0;
        char c;
        while ((c = fgetc(file)) != EOF) {//Read contents until it reach the end of the file
              buffer[i] = c;
              i++;
        }
        buffer[i] = '\0';
        fclose(file);
        printf("JSON: %s\n", buffer);

        ecs_entity_t test;
        test = ecs_new(gameplay_world);
        ecs_add(gameplay_world, test, Position);
        const char *json_entity = ecs_entity_from_json(gameplay_world, test, buffer, NULL);
        ecs_progress(gameplay_world, 0);
        const Position *p = ecs_get(gameplay_world, test, Position);
        printf("DES_JSON_ENTITY_START\n");
        printf("Deserialized TEST Position: .id %llu { .x = %d, .y = %d }", test, p->x, p->y);
        printf("\nDES_JSON_ENTITY_END\n");
    }
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
    /* TODO create gameplay_world components */
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
