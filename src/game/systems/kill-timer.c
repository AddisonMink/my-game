#include "kill-timer.h"

void KillTimerSystem(Entities *entities, float delta)
{
  for (int i = 0; i < MAX_ENTITIES; i++)
  {
    if (entities->active[i] && entities->killTimer[i].active)
    {
      KillTimer *killTimer = &entities->killTimer[i];
      killTimer->time += delta;
      if (killTimer->time >= killTimer->duration)
      {
        EntitiesDelete(entities, i);
      }
    }
  }
}