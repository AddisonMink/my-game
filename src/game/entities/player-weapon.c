#include "player-weapon.h"

#include "raymath.h"

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

  const Vector2 pos = ({
    const PlayerController *controller = &entities->controller[0].data.player;
    const Body *playerBody = &entities->body[0];

    const Vector2 playerCenter = BodyCenter(playerBody);
    const Vector2 directionVector = DirectionVector(controller->facing);
    const Vector2 playerEdge = Vector2Add(playerCenter, Vector2Scale(directionVector, playerBody->width / 2));
    const Vector2 offset = Vector2Scale(directionVector, playerBody->width / 2);
    Vector2Add(playerEdge, offset);
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
      .width = 0,
      .height = 0,
      .solid = false,
      .velocity = (Vector2){0, 0},
  };

  entities->killTimer[id] = (KillTimer){
      .active = true,
      .duration = timeToLive,
      .time = 0,
  };
}