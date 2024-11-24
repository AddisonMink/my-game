#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "raylib.h"

#include "resources/spritesheet.h"

typedef struct
{
    SpritesheetId spritesheet;
    AnimationId animation;
    float time;
} State;

State state = {
    .spritesheet = -1,
    .animation = -1,
    .time = 0,
};

static void init()
{
    SpritesheetLoad("player");
    state.spritesheet = SpritesheetGetId("player");
    state.animation = SpritesheeetGetAnimationId(state.spritesheet, "walk-down");
}

static void deinit()
{
    SpritesheetUnloadAll();
}

static void update()
{
    const float delta = GetFrameTime();
    state.time += delta;

    BeginDrawing();
    ClearBackground(BLACK);
    SpritesheetDrawAnimation(state.spritesheet, state.animation, state.time, (Vector2){0, 0});
    EndDrawing();
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "my-game");
    init();

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
    deinit();
    return 0;
}