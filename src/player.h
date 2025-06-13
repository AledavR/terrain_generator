#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"

class Player {
public:
  Vector3 position;
  Vector3 size;
  Vector3 velocity;
  float moveSpeed;
  float cameraRadius;
  float gravity;
  bool isOnGround;
  float stepHeight;

  Player(const Vector3& startPos);

  void Update(Camera& camera, const Model& tower);
  void Draw();
};

#endif // PLAYER_H
