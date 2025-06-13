#include "player.h"
#include "config.h"

Player::Player(const Vector3& startPos)
  : position(startPos),
    size({2.5f, 5.0f, 2.5f}),
    velocity({0.0f, 0.0f, 0.0f}),       
    moveSpeed(1.0f),
    gravity(0.05f),
    isOnGround(false),
    stepHeight(100.0f)
{}

void Player::Update(Camera& camera, const Model& model)
{

  Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
  forward.y = 0.0f;
  forward = Vector3Normalize(forward);

  Vector3 right = Vector3CrossProduct(forward, (Vector3){ 0.0f, 1.0f, 0.0f });  // right = forward × up

  Vector3 moveDir = { 0 };

  // WASD movement relative to camera
  if (IsKeyDown(KEY_W))
    {
      moveDir = Vector3Add(moveDir, forward);
      CameraMoveToTarget(&camera, -1.0f);
    }
  if (IsKeyDown(KEY_S))
    {
      moveDir = Vector3Subtract(moveDir, forward);
      CameraMoveToTarget(&camera, 0.15f);
    }
  if (IsKeyDown(KEY_D)) moveDir = Vector3Add(moveDir, right);
  if (IsKeyDown(KEY_A)) moveDir = Vector3Subtract(moveDir, right);

  if (Vector3Length(moveDir) > 0.0f)
    {
      moveDir = Vector3Normalize(moveDir);
    }

  Vector3 proposedPos = position;
  proposedPos.x += moveDir.x * moveSpeed;
  proposedPos.z += moveDir.z * moveSpeed;

  // // === Update camera to follow the player ===
  // camera.position = Vector3Add(position, (Vector3){ 6.0f, 3.0f, 0.0f });  // Above and behind
  camera.target = position;
  // camera.up = (Vector3){ 0.0f, 3.5f, 0.0f };
  
  // === Wall Collision Check ===
  Ray wallRay{
    .position = { position.x, position.y, position.z },
    .direction = Vector3Normalize(Vector3Subtract(proposedPos, position))
  };

  RayCollision wallHit = { 0 };
  for (int m = 0; m < model.meshCount; m++)
    {
      RayCollision hit = GetRayCollisionMesh(wallRay, model.meshes[m], model.transform);
      if (hit.hit && (!wallHit.hit || hit.distance < wallHit.distance))
        wallHit = hit;
    }

  const float minDistance = 0.4f;
  if (!(wallHit.hit && wallHit.distance < minDistance))
    {
      position.x = proposedPos.x;
      position.z = proposedPos.z;
    }

  // === Apply Gravity ===
  if (!isOnGround)
    velocity.y -= gravity;
  else
    velocity.y = 0;

  position.y += velocity.y;

  // === Ground Detection ===
  Ray downRay{
    .position = { position.x, position.y, position.z },
    .direction = { 0.0f, -1.0f, 0.0f }
  };

  RayCollision floorHit = { 0 };
  for (int m = 0; m < model.meshCount; m++)
    {
      RayCollision hit = GetRayCollisionMesh(downRay, model.meshes[m], model.transform);
      if (hit.hit && (!floorHit.hit || hit.distance < floorHit.distance))
        floorHit = hit;
    }

  float groundY = 0.0f;
  bool isOverMesh = false;
  
  if (floorHit.hit && floorHit.distance <= (size.y / 2 + 0.2f + stepHeight))
    {
      groundY = floorHit.point.y + size.y / 2;
      isOverMesh = true;
    }
  
  if (position.y < groundY)
    {
      position.y = groundY;
      isOnGround = true;
    }
  else if (!isOverMesh && position.y - size.y / 2 < 0.0f)
    {
      position.y = size.y / 2;
      isOnGround = true;
    }
  else
    {
      isOnGround = false;
    }
}



void Player::Draw()
{
  DrawCube(position, size.x, size.y, size.z, BLUE);
  DrawCubeWires(position, size.x, size.y, size.z, DARKBLUE);
}
