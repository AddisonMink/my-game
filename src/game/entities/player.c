#include "player.h"

#include "raymath.h"

static const float walkSpeed = 200;

static const char *DirectionString(Direction dir)
{
  static const char *directionString[] = {
      [UP] = "up",
      [DOWN] = "down",
      [LEFT] = "left",
      [RIGHT] = "right",
  };
  return directionString[dir];
}

void PlayerInit(Entities *entities, int x, int y)
{
  const EntityId id = 0;
  const Vector2 pos = (Vector2){x * SPRITE_SIZE_SCALED, y * SPRITE_SIZE_SCALED};

  entities->active[id] = true;

  const SpritesheetId spritesheet = SpritesheetGetId("player");
  const AnimationId animation = SpritesheetGetAnimationId(spritesheet, "idle-right");
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
      .velocity = (Vector2){0, 0},
  };

  entities->controller[id].active = true;
  entities->controller[id].type = CONTROLLER_PLAYER;
  entities->controller[id].data.player = (PlayerController){
      .facing = RIGHT,
      .idle = true,
  };
}

void PlayerUpdate(Entities *entities, float delta)
{
  TraceLog(LOG_INFO, "PlayerUpdate");

  PlayerController *controller = &entities->controller[0].data.player;
  Animator *animator = &entities->animator[0];
  Body *body = &entities->body[0];

  const Direction oldFacing = controller->facing;

  const Vector2 dir = ({
    Vector2 dir = {0, 0};
    if (IsKeyDown(KEY_D))
      dir.x += 1;
    if (IsKeyDown(KEY_A))
      dir.x -= 1;
    if (IsKeyDown(KEY_S))
      dir.y += 1;
    if (IsKeyDown(KEY_W))
      dir.y -= 1;
    dir;
  });

  const Direction newFacing = ({
    Direction newFacing = oldFacing;
    if (dir.x == 0 && dir.y != 0)
      newFacing = dir.y > 0 ? DOWN : UP;
    else if (dir.x != 0 && dir.y == 0)
      newFacing = dir.x > 0 ? RIGHT : LEFT;
    newFacing;
  });

  const Vector2 velocity = Vector2Scale(Vector2Normalize(dir), walkSpeed);
  const bool idle = Vector2Length(dir) == 0;
  const bool changedDirection = newFacing != oldFacing;
  const bool changedIdle = idle != controller->idle;

  const AnimationId animationId = ({
    AnimationId id = animator->animation;
    char *animationType;
    if (changedDirection)
      animationType = "walk";
    else if (changedIdle && idle)
      animationType = "idle";
    else if (changedIdle && !idle)
      animationType = "walk";
    else
      animationType = "none";
    if (!TextIsEqual(animationType, "none"))
      id = SpritesheetGetAnimationId(animator->spritesheet, TextFormat("%s-%s", animationType, DirectionString(newFacing)));
    id;
  });

  controller->facing = newFacing;
  controller->idle = idle;

  animator->animation = animationId;

  body->velocity = Vector2Scale(Vector2Normalize(dir), walkSpeed);
}