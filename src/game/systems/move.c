#include "move.h"

#include "raymath.h"

void MoveSystem(MapId map, Entities *entities, float delta)
{
  for (int i = 0; i < MAX_ENTITIES; i++)
  {
    if (!entities->active[i] || !entities->body[i].active)
      continue;

    Body *body = &entities->body[i];
    const Rectangle rect = BodyRect(body);

    const Rectangle next = {
        rect.x + body->velocity.x * delta,
        rect.y + body->velocity.y * delta,
        rect.width,
        rect.height,
    };

    if (MapCheckCollision(map, next))
      continue;

    body->pos = Vector2Add(body->pos, Vector2Scale(body->velocity, delta));
  }
}