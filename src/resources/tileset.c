#include "tileset.h"

#include "json/json.h"
#include "spritesheet.h"

#define MAX_TILESETS 1
#define MAX_TILES 32

typedef struct
{
  bool walkable;
} Tile;

typedef struct
{
  char name[32];
  SpritesheetId spritesheet;
  int tileCount;
  Tile tiles[MAX_TILES];
} Tileset;

static bool loaded[MAX_TILESETS] = {false};
static Tileset tilesets[MAX_TILESETS];

#pragma region LOADING

static bool extractTile(void *result, const Json *json)
{
  Tile *tile = (Tile *)result;
  return JsonObjectExtract(&tile->walkable, json, "walkable", JsonExtractBool);
}

static bool extractTileset(void *result, const Json *json)
{
  Tileset *tileset = (Tileset *)result;
  return JsonObjectExtract(tileset->name, json, "name", JsonExtractString) &&
         JsonObjectExtractArray(&tileset->tiles, &tileset->tileCount, sizeof(Tile), json, "tiles", extractTile);
}

void TilesetLoad(const char *name)
{
  TilesetId id = -1;
  for (int i = 0; i < MAX_TILESETS; i++)
  {
    if (!loaded[i])
    {
      id = i;
      break;
    }
  }
  if (id == -1)
  {
    TraceLog(LOG_ERROR, "No more tilesets can be loaded.");
    return;
  }

  Json *json = JsonLoadFile(TextFormat("resources/maps/%s.json", name));
  if (!extractTileset(&tilesets[id], json))
  {
    TraceLog(LOG_ERROR, "Failed to load tileset: %s", name);
    JsonUnload(json);
    return;
  }

  tilesets[id].spritesheet = SpritesheetGetId(tilesets[id].name);
  if (tilesets[id].spritesheet == -1)
  {
    TraceLog(LOG_ERROR, "Failed to load spritesheet for tileset: %s", name);
    JsonUnload(json);
    return;
  }

  loaded[id] = true;
  JsonUnload(json);
}

#pragma endregion

#pragma region PUBLIC

TilesetId TilesetGetId(const char *name)
{
  for (int i = 0; i < MAX_TILESETS; i++)
  {
    if (loaded[i] && TextIsEqual(tilesets[i].name, name))
    {
      return i;
    }
  }

  return -1;
}

bool TileIsWalkable(TilesetId id, TileId tile)
{
  if (tile == -1)
    return true;
  return tilesets[id].tiles[tile].walkable;
}

void TilesetDrawTile(TilesetId id, TileId tile, Vector2 position)
{
  if (tile == -1)
    return;
  SpritesheetDrawFrame(tilesets[id].spritesheet, tile, position);
}

void TilesetUnloadAll()
{
  for (int i = 0; i < MAX_TILESETS; i++)
  {
    if (loaded[i])
    {
      loaded[i] = false;
    }
  }
}

#pragma endregion