#include "player-weapon.h"

#include "raymath.h"

static const float length = 14 * SPRITE_SCALE;
static const float breadth = 8 * SPRITE_SCALE;
static const float offset = 2 * SPRITE_SCALE;

static Vector2 DirectionVector(Direction dir)
{
  static const Vector2 directionVector[] = {
      [UP] = {0, -1},
      [DOWN] = {0, 1},
      [LEFT] = {-1, 0},
      [RIGHT] = {1, 0},
  };
  return directionVector[dir];
};

void PlayerWeaponInit(Entities *entities, float timeToLive)
{
  const EntityId id = 1;
  const Direction facing = entities->controller[0].data.player.facing;

  const SpritesheetId spritesheet = SpritesheetGetId("player-tools");
  const AnimationId animation = SpritesheetGetAnimationId(spritesheet, TextFormat("sword-%s", DirectionToString(facing)));

  float width;
  float height;
  const Vector2 pos = ({
    const Body *playerBody = &entities->body[0];
    const Vector2 playerCenter = BodyCenter(playerBody);

    Vector2 pos = {0, 0};
    switch (facing)
    {
    case UP:
      width = breadth;
      height = length;
      pos = (Vector2){
          playerCenter.x - width / 2,
          playerCenter.y - playerBody->height / 2 - height + offset,
      };
      break;
    case DOWN:
      width = breadth;
      height = length;
      pos = (Vector2){
          playerCenter.x - width / 2,
          playerCenter.y + playerBody->height / 2 - offset,
      };
      break;
    case LEFT:
      width = length;
      height = breadth;
      pos = (Vector2){
          playerCenter.x - playerBody->width / 2 - width + offset,
          playerCenter.y - height / 2,
      };
      break;
    case RIGHT:
      width = length;
      height = breadth;
      pos = (Vector2){
          playerCenter.x + playerBody->width / 2 - offset,
          playerCenter.y - height / 2,
      };
      break;
    }
    pos;
  });

  entities->active[id] = true;

  entities->animator[id] = (Animator){
      .active = true,
      .spritesheet = spritesheet,
      .animation = animation,
      .time = 0,
  };

  entities->body[id] = (Body){
      .active = true,
      .pos = pos,
      .width = width,
      .height = height,
      .solid = false,
      .velocity = (Vector2){0, 0},
  };

  entities->killTimer[id] = (KillTimer){
      .active = true,
      .duration = timeToLive,
      .time = 0,
  };
}