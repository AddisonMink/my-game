#include "system.h"

#include "game/entities/player.h"
#include "animate.h"
#include "draw.h"
#include "move.h"

void SystemUpdate(MapId map, Entities *entities, float delta)
{
  for (int i = 0; i < MAX_ENTITIES; i++)
  {
    if (!entities->active[i] || !entities->controller[i].active)
      continue;

    switch (entities->controller[i].type)
    {
    case CONTROLLER_PLAYER:
      PlayerUpdate(entities, delta);
      break;
    }
  }

  MoveSystem(map, entities, delta);
  AnimateSystem(entities, delta);

  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawSystem(map, entities);
  DrawFPS(10, 10);
  EndDrawing();
}