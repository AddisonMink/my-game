#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "raylib.h"

#include "resources/spritesheet.h"
#include "resources/tileset.h"
#include "resources/map.h"
#include "game/components.h"
#include "game/entities/player.h"
#include "game/systems/system.h"

MapId map;
Entities entities;

static void init()
{
    SpritesheetLoad("player");
    SpritesheetLoad("forest-tiles");
    TilesetLoad("forest-tiles");
    MapLoad("test-room");
    map = MapGetId("test-room");
    EntitiesInit(&entities);
    PlayerInit(&entities, 1, 1);
}

static void deinit()
{
    SpritesheetUnloadAll();
    TilesetUnloadAll();
    MapUnloadAll();
}

static void update()
{
    const float delta = GetFrameTime();
    SystemUpdate(map, &entities, delta);
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