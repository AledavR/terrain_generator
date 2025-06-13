#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "raylib.h"

class Player {
public:
    Vector3 position;
    Vector3 size;
    Vector3 velocity;
    float moveSpeed;
    float gravity;

    Player(const Vector3& startPos);

    void Update(Camera& camera, const Image& heightmap);
    void Draw();
};

#endif // PLAYER_HPP
