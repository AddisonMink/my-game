#pragma once

#include "resources/spritesheet.h"

#define MAX_ENTITIES 10

typedef int EntityId;

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

#pragma endregion

#pragma region CONTROLLERS

typedef struct
{

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
  Controller controller[MAX_ENTITIES];
} Entities;

void EntitiesInit(Entities *entities);

#pragma endregion
