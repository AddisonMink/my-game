#pragma once

#define MAP_WIDTH 11
#define MAP_HEIGHT 8
#define MAP_SIZE (MAP_WIDTH * MAP_HEIGHT)

typedef int MapId;

void MapLoad(const char *name);

MapId MapGetId(const char *name);

void MapDraw(MapId id);

void MapUnloadAll();