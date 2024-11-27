#include "draw.h"

void DrawSystem(MapId map, const Entities *entities)
{
  MapDraw(map);

  for (int l = 0; l < MAX_ANIMATION_LAYERS; l++)
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
      if (!entities->active[i] || !entities->animator[i].active || entities->animator[i].layer != l)
        continue;

      const Body *body = &entities->body[i];
      const Animator *animator = &entities->animator[i];

      const Vector2 center = BodyCenter(body);
      const Vector2 pos = (Vector2){center.x - SPRITE_SIZE_SCALED / 2, center.y - SPRITE_SIZE_SCALED / 2};
      SpritesheetDrawAnimation(animator->spritesheet, animator->animation, animator->time, pos);

      DrawRectangleLinesEx(BodyRect(body), 2, RED);
    }
}