#include "player.h"
#include "config.h"

Player::Player(const Vector3& startPos)
  : position(startPos),
    size({1.5f, 4.5f, 1.5f}),
    velocity({0.0f, 0.0f, 0.0f}),       
    moveSpeed(1.0f),
    cameraRadius(30.0f),
    gravity(0.05f),
    isOnGround(false),
    stepHeight(100.0f)
{}

void Player::Update(Camera& camera, const Model& model)
{
  static float yaw = 0.0f;
  static float pitch = 9.0f;

  Vector2 mouse = GetMouseDelta();
  float sensitivityX = 0.003f;
  float sensitivityY = 0.003f;

  yaw += mouse.x * sensitivityX;
  pitch -= mouse.y * sensitivityY;

  // Limitar el pitch para no voltear la cámara
  if (pitch > 1.5f) pitch = 1.5f;
  if (pitch < -1.5f) pitch = -1.5f;

  float wheelMove = GetMouseWheelMove();

  if (wheelMove > 0)  cameraRadius -= 1.0f;
  else if (wheelMove < 0) cameraRadius += 1.0f; 

  // Calculate camera position based on spherical coordinates
  float camX = sinf(yaw) * cosf(pitch) * cameraRadius;
  float camY = sinf(pitch) * cameraRadius;
  float camZ = cosf(yaw) * -cosf(pitch) * cameraRadius;

  Vector3 forward = Vector3Normalize((Vector3){ camX, 0.0f, camZ });
  Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

  Vector3 moveDir = { 0 };

  // WASD movement relative to camera
  if (IsKeyDown(KEY_W)) moveDir = Vector3Subtract(moveDir, forward);
  if (IsKeyDown(KEY_S)) moveDir = Vector3Add(moveDir, forward);
  if (IsKeyDown(KEY_D)) moveDir = Vector3Subtract(moveDir, right);
  if (IsKeyDown(KEY_A)) moveDir = Vector3Add(moveDir, right);
  if (IsKeyDown(KEY_SPACE)) velocity.y = 2.0f;

  if (Vector3Length(moveDir) > 0.0f) moveDir = Vector3Normalize(moveDir);

  Vector3 proposedPos = position;
  proposedPos.x += moveDir.x * moveSpeed;
  proposedPos.z += moveDir.z * moveSpeed;
  
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

  velocity.y = Clamp(velocity.y, -2.0f, 10.0f);
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
  else { isOnGround = false; }

  camera.position = (Vector3){
    position.x + camX,
    position.y + camY + 2.0f,
    position.z + camZ
  };
  camera.target = position;
}

void Player::Draw()
{
  DrawCube(position, size.x, size.y, size.z, BLUE);
  DrawCubeWires(position, size.x, size.y, size.z, DARKBLUE);
}
