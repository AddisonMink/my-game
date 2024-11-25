#include "player.h"

void PlayerInit(Entities *entities, int x, int y)
{
  const EntityId id = 0;
  const Vector2 pos = (Vector2){x * SPRITE_SIZE_SCALED, y * SPRITE_SIZE_SCALED};

  entities->active[id] = true;

  const SpritesheetId spritesheet = SpritesheetGetId("player");
  const AnimationId animation = SpritesheetGetAnimationId(spritesheet, "walk-right");
  entities->animator[id] = (Animator){
      .active = true,
      .spritesheet = spritesheet,
      .animation = animation,
      .time = 0,
  };

  entities->body[id] = (Body){
      .active = true,
      .pos = pos,
      .width = 48,
      .height = 48,
      .solid = true,
      .velocity = (Vector2){100, 0},
  };

  entities->controller[id].active = true;
  entities->controller[id].type = CONTROLLER_PLAYER;
  entities->controller[id].data.player = (PlayerController){};
}