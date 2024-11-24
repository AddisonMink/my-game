#pragma once

#include "raylib.h"

#define SPRITE_SIZE 16
#define SPRITE_SCALE 4
#define SPRITE_SIZE_SCALED (SPRITE_SIZE * SPRITE_SCALE)

typedef int SpritesheetId;

typedef int AnimationId;

void SpritesheetLoad(const char *name);

SpritesheetId SpritesheetGetId(const char *name);

AnimationId SpritesheeetGetAnimationId(SpritesheetId id, const char *name);

void SpritesheetDrawFrame(SpritesheetId id, int frame, Vector2 position);

void SpritesheetDrawAnimation(SpritesheetId id, AnimationId animation, float time, Vector2 position);

void SpritesheetUnloadAll();
