#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "raylib.h"

#include "resources/spritesheet.h"
#include "resources/tileset.h"
#include "resources/map.h"

typedef struct
{
    SpritesheetId spritesheet;
    AnimationId animation;
    MapId map;
    float time;
} State;

State state = {
    .spritesheet = -1,
    .animation = -1,
    .map = -1,
    .time = 0,
};

static void init()
{
    SpritesheetLoad("player");
    SpritesheetLoad("forest-tiles");
    TilesetLoad("forest-tiles");
    MapLoad("test-room");
    state.spritesheet = SpritesheetGetId("player");
    state.animation = SpritesheeetGetAnimationId(state.spritesheet, "walk-down");
    state.map = MapGetId("test-room");
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
    ClearBackground(RAYWHITE);
    MapDraw(state.map);
    SpritesheetDrawAnimation(state.spritesheet, state.animation, state.time, (Vector2){100, 100});
    EndDrawing();
}

int main(void)
{
    const int screenWidth = MAP_WIDTH * SPRITE_SIZE_SCALED;
    const int screenHeight = MAP_HEIGHT * SPRITE_SIZE_SCALED;
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