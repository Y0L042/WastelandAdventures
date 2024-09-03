#include <raylib.h>
#include "../src/tween.h"

ecs_world_t *world;
ECS_COMPONENT_DECLARE(TweenComponent);
ECS_COMPONENT_DECLARE(TweenPropertyComponent);

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);               

	world = ecs_init();
	ECS_COMPONENT_DECLARE(TweenComponent);
	ECS_COMPONENT_DECLARE(TweenPropertyComponent);
	ECS_COMPONENT_DEFINE(world, TweenComponent);
	ECS_COMPONENT_DEFINE(world, TweenPropertyComponent);

    while (!WindowShouldClose())    
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
