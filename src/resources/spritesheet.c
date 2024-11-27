#include "spritesheet.h"

#include "raymath.h"

#include "json/json.h"

#define MAX_SPRITESHEETS 16
#define MAX_ANIMATIONS 16
#define ANIMATION_FPS 6

#pragma region TYPES

typedef struct
{
  char name[32];
  int start;
  int stop;
  bool flipX;
  bool flipY;
  int offsetX;
  int offsetY;
} Animation;

typedef struct
{
  char name[32];
  Texture2D texture;
  int rows;
  int cols;
  Animation animations[MAX_ANIMATIONS];
  int animationsCount;
} Spritesheet;

static bool loaded[MAX_SPRITESHEETS] = {false};
static Spritesheet spritesheets[MAX_SPRITESHEETS];

#pragma endregion

#pragma region LOADING

static bool extractAnimation(void *result, const Json *json)
{
  Animation *animation = (Animation *)result;

  return JsonObjectExtract(animation->name, json, "name", JsonExtractString) &&
         JsonObjectExtract(&animation->start, json, "start", JsonExtractInt) &&
         JsonObjectExtract(&animation->stop, json, "stop", JsonExtractInt) &&
         JsonObjectExtract(&animation->flipX, json, "flipX", JsonExtractBool) &&
         JsonObjectExtract(&animation->flipY, json, "flipY", JsonExtractBool) &&
         JsonObjectExtract(&animation->offsetX, json, "offsetX", JsonExtractInt) &&
         JsonObjectExtract(&animation->offsetY, json, "offsetY", JsonExtractInt);
}

static bool extractSpritesheet(void *result, const Json *json)
{
  Spritesheet *sheet = (Spritesheet *)result;

  if (!JsonObjectExtract(sheet->name, json, "name", JsonExtractString))
    return false;

  sheet->texture = LoadTexture(TextFormat("resources/spritesheets/%s.png", sheet->name));

  return JsonObjectExtract(&sheet->rows, json, "rows", JsonExtractInt) &&
         JsonObjectExtract(&sheet->cols, json, "cols", JsonExtractInt) &&
         JsonObjectExtractArray(sheet->animations, &sheet->animationsCount, sizeof(Animation), json, "animations", extractAnimation);
}

void SpritesheetLoad(const char *name)
{
  SpritesheetId id = -1;
  for (int i = 0; i < MAX_SPRITESHEETS; i++)
  {
    if (!loaded[i])
    {
      id = i;
      break;
    }
  }

  if (id == -1)
  {
    TraceLog(LOG_ERROR, "No more spritesheets available.");
    return;
  }

  Json *json = JsonLoadFile(TextFormat("resources/spritesheets/%s.json", name));
  if (!json)
  {
    TraceLog(LOG_ERROR, "Failed to load spritesheet: %s", name);
    return;
  }

  extractSpritesheet(&spritesheets[id], json);
  loaded[id] = true;
  JsonUnload(json);
}

#pragma endregion

#pragma region PUBLiC

SpritesheetId SpritesheetGetId(const char *name)
{
  for (int i = 0; i < MAX_SPRITESHEETS; i++)
  {
    if (loaded[i] && TextIsEqual(spritesheets[i].name, name))
    {
      return i;
    }
  }

  TraceLog(LOG_ERROR, "Spritesheet not found: %s", name);
  return -1;
}

AnimationId SpritesheetGetAnimationId(SpritesheetId id, const char *name)
{
  const Spritesheet *sheet = &spritesheets[id];
  for (int i = 0; i < sheet->animationsCount; i++)
  {
    if (TextIsEqual(sheet->animations[i].name, name))
    {
      return i;
    }
  }

  TraceLog(LOG_ERROR, "Animation not found: %s#%s", sheet->animations[id].name, name);
  return -1;
}

static void drawFrameFlipped(SpritesheetId id, int frame, Vector2 position, bool flipX, bool flipY)
{
  const Spritesheet *sheet = &spritesheets[id];
  const int row = frame / sheet->cols;
  const int col = frame % sheet->cols;

  const Rectangle source = {
      col * SPRITE_SIZE,
      row * SPRITE_SIZE,
      flipX ? -SPRITE_SIZE : SPRITE_SIZE,
      flipY ? -SPRITE_SIZE : SPRITE_SIZE,
  };

  const Rectangle dest = {
      position.x,
      position.y,
      SPRITE_SIZE * SPRITE_SCALE,
      SPRITE_SIZE * SPRITE_SCALE,
  };

  DrawTexturePro(sheet->texture, source, dest, (Vector2){0, 0}, 0, WHITE);
}

void SpritesheetDrawFrame(SpritesheetId id, int frame, Vector2 position)
{
  drawFrameFlipped(id, frame, position, false, false);
}

void SpritesheetDrawAnimation(SpritesheetId id, AnimationId animation, float time, Vector2 position)
{
  const Spritesheet *sheet = &spritesheets[id];
  const Animation *anim = &sheet->animations[animation];
  const int frame = (int)(time * ANIMATION_FPS) % (anim->stop - anim->start + 1) + anim->start;
  const Vector2 pos = Vector2Add(position, Vector2Scale((Vector2){anim->offsetX, anim->offsetY}, SPRITE_SCALE));
  drawFrameFlipped(id, frame, pos, anim->flipX, anim->flipY);
}

void SpritesheetUnloadAll()
{
  for (int i = 0; i < MAX_SPRITESHEETS; i++)
  {
    if (loaded[i])
    {
      UnloadTexture(spritesheets[i].texture);
      loaded[i] = false;
    }
  }
}

#pragma endregion