#include "player.h"

#include "player-weapon.h"

#include "raymath.h"

#pragma region CONSTANTS

static const float walkSpeed = 200;

static const float attackDuration = 0.25;

#pragma endregion

#pragma region INITI

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
      .state = PLAYER_WALKING,
      .facing = RIGHT,
      .idle = true,
  };
}

#pragma endregion

#pragma region DECLARATIONS
static void walkingStart(Entities *entities);
static void walkingUpdate(Entities *entities, float delta);
static void attackingStart(Entities *entities);
static void attackingUpdate(Entities *entities, float delta);

#pragma region WALKING

static void walkingStart(Entities *entities)
{
  PlayerController *controller = &entities->controller[0].data.player;
  Animator *animator = &entities->animator[0];

  controller->state = PLAYER_WALKING;
  controller->idle = true;
  animator->animation = SpritesheetGetAnimationId(animator->spritesheet, TextFormat("idle-%s", DirectionToString(controller->facing)));
}

static void walkingUpdate(Entities *entities, float delta)
{
  PlayerController *controller = &entities->controller[0].data.player;
  Animator *animator = &entities->animator[0];
  Body *body = &entities->body[0];

  if (IsKeyPressed(KEY_J))
  {
    attackingStart(entities);
    return;
  }

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
      id = SpritesheetGetAnimationId(animator->spritesheet, TextFormat("%s-%s", animationType, DirectionToString(newFacing)));
    id;
  });

  controller->facing = newFacing;
  controller->idle = idle;

  if (animationId != animator->animation)
  {
    animator->animation = animationId;
    animator->time = 0;
  }

  body->velocity = Vector2Scale(Vector2Normalize(dir), walkSpeed);
}

#pragma endregion

#pragma region ATTACKING

static void attackingStart(Entities *entities)
{
  PlayerController *controller = &entities->controller[0].data.player;
  Animator *animator = &entities->animator[0];
  Body *body = &entities->body[0];

  controller->state = PLAYER_ATTACKING;
  controller->attackTime = 0;
  animator->animation = SpritesheetGetAnimationId(animator->spritesheet, TextFormat("attack-%s", DirectionToString(controller->facing)));
  body->velocity = (Vector2){0, 0};

  PlayerWeaponInit(entities);
}

static void attackingUpdate(Entities *entities, float delta)
{
  PlayerController *controller = &entities->controller[0].data.player;

  controller->attackTime += delta;

  if (controller->attackTime >= attackDuration)
  {
    walkingStart(entities);
  }
}

#pragma endregion

void PlayerUpdate(Entities *entities, float delta)
{
  const PlayerController *controller = &entities->controller[0].data.player;
  switch (controller->state)
  {
  case PLAYER_WALKING:
    return walkingUpdate(entities, delta);
  case PLAYER_ATTACKING:
    return attackingUpdate(entities, delta);
  }
}