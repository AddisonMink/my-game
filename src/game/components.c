#include "components.h"

#pragma region BODY

Rectangle BodyRect(const Body *body)
{
  return (Rectangle){body->pos.x, body->pos.y, body->width, body->height};
}

Vector2 BodyCenter(const Body *body)
{
  return (Vector2){body->pos.x + body->width / 2, body->pos.y + body->height / 2};
}

#pragma endregion

#pragma region ENTITIES

void EntitiesInit(Entities *entities)
{
  for (int i = 0; i < MAX_ENTITIES; i++)
  {
    entities->active[i] = false;
    entities->animator[i].active = false;
    entities->body[i].active = false;
    entities->controller[i].active = false;
  }
}

#pragma endregion