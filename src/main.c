#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "raylib.h"

#include "module/module.h"

static void update()
{
    const float delta = GetFrameTime();
    // Update here.

    BeginDrawing();
    ClearBackground(BLACK);
    Module_Func();
    EndDrawing();
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Template-5.0.0");
    // Initialize here.

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(update, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        update();
    }
#endif

    CloseWindow();
    // Deinitialize here.
    return 0;
}