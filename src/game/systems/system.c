#include "system.h"

#include "animate.h"
#include "draw.h"
#include "move.h"

void SystemUpdate(MapId map, Entities *entities, float delta)
{
  MoveSystem(map, entities, delta);
  AnimateSystem(entities, delta);

  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawSystem(map, entities);
  EndDrawing();
}