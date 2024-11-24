#pragma once

#include "raylib.h"

typedef int TilesetId;

typedef int TileId;

void TilesetLoad(const char *name);

TilesetId TilesetGetId(const char *name);

bool TileIsWalkable(TilesetId id, TileId tile);

void TilesetDrawTile(TilesetId id, TileId tile, Vector2 position);

void TilesetUnloadAll();