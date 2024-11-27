#pragma once

#include "resources/spritesheet.h"

#define MAX_ENTITIES 10

typedef int EntityId;

typedef enum
{
  LEFT,
  RIGHT,
  UP,
  DOWN
} Direction;

const char *DirectionToString(Direction dir);

#pragma region BASIC_COMPONENTS

typedef struct
{
  bool active;
  SpritesheetId spritesheet;
  AnimationId animation;
  float time;
} Animator;

typedef struct
{
  bool active;
  Vector2 pos;
  float width;
  float height;
  bool solid;
  Vector2 velocity;
} Body;

Rectangle BodyRect(const Body *body);

Vector2 BodyCenter(const Body *body);

typedef struct
{
  bool active;
  float time;
  float duration;
} KillTimer;

#pragma endregion

#pragma region CONTROLLERS

typedef struct
{
  enum
  {
    PLAYER_WALKING,
    PLAYER_ATTACKING,
  } state;
  Direction facing;
  bool idle;
  float attackTime;
} PlayerController;

typedef struct
{
  bool active;
  enum
  {
    CONTROLLER_PLAYER
  } type;
  union
  {
    PlayerController player;
  } data;
} Controller;

#pragma endregion

#pragma region ENTITIES

typedef struct
{
  bool active[MAX_ENTITIES];
  Animator animator[MAX_ENTITIES];
  Body body[MAX_ENTITIES];
  KillTimer killTimer[MAX_ENTITIES];
  Controller controller[MAX_ENTITIES];
} Entities;

void EntitiesInit(Entities *entities);

void EntitiesDelete(Entities *entities, EntityId id);

#pragma endregion
