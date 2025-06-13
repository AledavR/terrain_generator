#include "player.hpp"
#include "terrain.h"
#include "config.h"
#include "raymath.h"
#include <cmath>

Player::Player(const Vector3& startPos) {
    position = startPos;
    size = (Vector3){ 1.0f, 2.0f, 1.0f };
    velocity = (Vector3){ 0 };
    moveSpeed = 0.5f;
    gravity = -0.98f;
}

void Player::Update(Camera& camera, const Image& heightmap) {
    static float yaw = 0.0f;
    static float pitch = 10.0f;  // empieza con un poco de inclinación

    Vector2 mouse = GetMouseDelta();
    float sensitivity = 0.003f;

    yaw -= mouse.x * sensitivity;
    pitch -= mouse.y * sensitivity;

    // Limitar el pitch para no voltear la cámara
    if (pitch > 1.5f) pitch = 1.5f;
    if (pitch < -1.5f) pitch = -1.5f;

    float radius = 8.0f;
    float camX = sinf(yaw) * cosf(pitch) * radius;
    float camY = sinf(pitch) * radius;
    float camZ = cosf(yaw) * cosf(pitch) * radius;

    Vector3 forward = Vector3Normalize((Vector3){ camX, 0.0f, camZ });
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

    Vector3 moveDir = { 0 };
    if (IsKeyDown(KEY_W)) moveDir = Vector3Add(moveDir, forward);
    if (IsKeyDown(KEY_S)) moveDir = Vector3Subtract(moveDir, forward);
    if (IsKeyDown(KEY_A)) moveDir = Vector3Subtract(moveDir, right);
    if (IsKeyDown(KEY_D)) moveDir = Vector3Add(moveDir, right);

    moveDir = Vector3Normalize(moveDir);
    moveDir = Vector3Scale(moveDir, moveSpeed);

    position.x += moveDir.x;
    position.z += moveDir.z;

    float offset = WORLD_SIZE / 2.0f;
    int i = (int)((position.x + offset) / TERRAIN_SCALE);
    int j = (int)((position.z + offset) / TERRAIN_SCALE);
    float h = GetHeightAtPoint(heightmap, i, j);

    position.y = h * MAX_TERRAIN_HEIGHT * TERRAIN_SCALE + size.y / 2.0f;

    camera.position = (Vector3){
        position.x + camX,
        position.y + camY + 2.0f,
        position.z + camZ
    };
    camera.target = position;
}

void Player::Draw() {
    DrawCube(position, size.x, size.y, size.z, RED);
    DrawCubeWires(position, size.x, size.y, size.z, BLACK);
}
