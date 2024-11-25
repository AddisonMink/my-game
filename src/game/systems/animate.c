#include "animate.h"

void AnimateSystem(Entities *entities, float delta)
{
  for (int i = 0; i < MAX_ENTITIES; i++)
  {
    if (!entities->active[i] || !entities->animator[i].active)
      continue;

    Animator *animator = &entities->animator[i];
    animator->time += delta;
  }
}