#include "map.h"

#include "json/json.h"
#include "spritesheet.h"
#include "tileset.h"

#define MAX_MAPS 1

typedef struct
{
  char name[32];
  TilesetId tileset;
  TileId tiles[MAP_SIZE];
} Map;

static bool loaded[MAX_MAPS] = {false};
static Map maps[MAX_MAPS];

#pragma region LOADING

static bool extractMap(void *result, const Json *json)
{
  Map *map = (Map *)result;

  char tilesetName[32];
  int count;

  if (!JsonObjectExtract(tilesetName, json, "tileset", JsonExtractString))
  {
    return false;
  }
  map->tileset = TilesetGetId(tilesetName);

  return JsonObjectExtract(map->name, json, "name", JsonExtractString) &&
         JsonObjectExtractArray(map->tiles, &count, sizeof(TileId), json, "tiles", JsonExtractInt);
}

void MapLoad(const char *name)
{
  MapId id = -1;
  for (int i = 0; i < MAX_MAPS; i++)
  {
    if (!loaded[i])
    {
      id = i;
      break;
    }
  }
  if (id == -1)
  {
    TraceLog(LOG_ERROR, "Map capacity reached.");
    return;
  }

  Json *json = JsonLoadFile(TextFormat("resources/maps/%s.json", name));
  extractMap(&maps[id], json);
  JsonUnload(json);
  loaded[id] = true;
}

#pragma endregion

#pragma region PUBLIC

MapId MapGetId(const char *name)
{
  for (int i = 0; i < MAX_MAPS; i++)
  {
    if (loaded[i] && TextIsEqual(maps[i].name, name))
    {
      return i;
    }
  }

  TraceLog(LOG_ERROR, "Map '%s' not found.", name);
  return -1;
}

void MapDraw(MapId id)
{
  const Map *map = &maps[id];

  for (int i = 0; i < MAP_SIZE; i++)
  {
    const int x = i % MAP_WIDTH * SPRITE_SIZE_SCALED;
    const int y = i / MAP_WIDTH * SPRITE_SIZE_SCALED;
    TilesetDrawTile(map->tileset, map->tiles[i], (Vector2){x, y});
  }
}

void MapUnloadAll()
{
  for (int i = 0; i < MAX_MAPS; i++)
  {
    loaded[i] = false;
  }
}

#pragma endregion