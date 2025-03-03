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
#include "grid.h"
#include "glyph.h"

#define PLATFORM_DESKTOP
#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

/* --- Constants --- */
static const char *STATE_NAME = "STATE_GAMEPLAYLOOP\0";

/* --- Variables --- */
int TILE_SCREEN_SIZE = 50;
const char *TEST_JSON_FILE = "C:\\Projects\\C_Projects\\WastelandAdventures\\data\\test.json";
const char *ENTITY_JSON_FILE = "C:\\Projects\\C_Projects\\WastelandAdventures\\data\\entities.json";
ecs_world_t *gameplay_world;
ecs_entity_t gameplay_camera2d;
Tileset gameplay_tileset;

static int is_gameplayloop_initialized = 0;

/* --- TESTING --- */
ecs_entity_t player;
Glyph test_glyph;
ecs_ref_t *camera_ref;
Grid *grid;
Shader test_shader;

/* --- SM Function Prototypes --- */
static void enter_state(void);
static void input();
static void update(double delta);
static void physics_update(double delta);
static void handle_ui(double delta);
static void draw(double delta);
static void draw_debug(double delta);
static void exit_state(void);

/* --- Function Prototypes --- */
static void initialize();
static int create_gameplay_world();
static int free_gameplay_world();

void state_gameplayloop_register(SM_Machine *sm, SM_State *state)
{
    state->state_enter = enter_state;
    state->state_input = input;
    state->state_update = update;
    state->state_physics_update = physics_update;
    state->state_handle_ui = handle_ui;
    state->state_draw = draw;
    state->state_draw_debug = draw_debug;
    state->state_exit = exit_state;

    sm_register_state(sm, state, STATE_NAME);
}

static void initialize()
{
    is_gameplayloop_initialized = 1;
    create_gameplay_world();

    grid = grid_create(gameplay_world, 25, 25, TILE_SCREEN_SIZE, TILE_SCREEN_SIZE);
    tileset_initialize(&gameplay_tileset, 
                       "./assets/RDE_8x8.png",
                       8, 8,
                       TILE_SCREEN_SIZE, TILE_SCREEN_SIZE,
                       WHITE);
    glyph_initialize(&test_glyph, &gameplay_tileset, 8, 8, WHITE);


    camera_ref = create_camera2d_pro(gameplay_world, (Camera2D){ 
        .offset = { SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f }, 
        .zoom = 1.0f 
    });
    set_active_camera2d(gameplay_world, camera_ref);
    camera2dcomponent_set_target(camera_ref, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

    entity_table_initialize();
    load_entity_definitions_from_file(TEST_JSON_FILE);

    player = create_entity_from_table(gameplay_world, "Player");
    Glyph player_glyph = glyph_create(&gameplay_tileset, 8, 8, WHITE);
    ecs_set(gameplay_world, player, Position, {50, 50});
    ecs_set(gameplay_world, player, Glyph, {
            .source_tile_x = player_glyph.source_tile_x,
            .source_tile_y = player_glyph.source_tile_y,
            .color = player_glyph.color,
            ._init_color = player_glyph._init_color,
            .tileset = player_glyph.tileset
        });
    if (player < 10) {
        print_entity_error(player);
    } else {
        const Position *p = ecs_get(gameplay_world, player, Position);
        log_info("Player: %llu, Position{ %.1f, %.1f }", player, p->x, p->y);
    }

    test_shader = LoadShader(
            0,
            TextFormat("resources/shaders/glsl%i/test_shader.fs", GLSL_VERSION));
}

static void enter_state(void)
{
    log_info("\n\nGAMEPLAYLOOP_ENTER\n\n");

    if (!is_gameplayloop_initialized) {
        initialize();
    }

}

static void input()
{
    static int mousewheel_y = 0;
    const float ZOOM_FACTOR = 0.1f;
    // camera2dcomponent_set_zoom(camera_ref, mousewheel_y * ZOOM_FACTOR);
    // mousewheel_y = GetMouseWheelMove();
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
    static Texture2D texture;
    static int setup = 0;
    if (!setup) {
        setup = 1;

        Image plain_img = GenImageColor(SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
        texture = LoadTextureFromImage(plain_img);
    }

    // glyph_draw(&test_glyph, 4, 4);
    grid_draw(grid);
}

static void draw_debug(double delta)
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
