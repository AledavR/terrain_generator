#include "forest.h"
#include <stdlib.h>
#include <math.h>

// Genera posiciones válidas para árboles flotantes (para depuración visual)
int GenerateForest(Vector3 *positions, Image heightmap) {
    int treesPlaced = 0;
    int maxAttempts = TREE_COUNT * 20;
    int attempts = 0;

    Vector3 offset = { -WORLD_SIZE / 2, 0, -WORLD_SIZE / 2 };

    while (treesPlaced < TREE_COUNT && attempts < maxAttempts) {
        int x = rand() % TERRAIN_SIZE;
        int z = rand() % TERRAIN_SIZE;

        float height = GetHeightAtPoint(heightmap, x, z, MAX_TERRAIN_HEIGHT);

        if (height >= 1.0f && height <= 90.0f) {
            Color pixel = GetImageColor(heightmap, x, z);
            // Condiciones para evitar agua y nieve
        if (pixel.r > 30 && pixel.r < 200) {
            positions[treesPlaced++] = (Vector3){
            x * TERRAIN_SCALE + offset.x,
            height + 40.0f,
            z * TERRAIN_SCALE + offset.z
    };
}

        }

        attempts++;
    }

    if (treesPlaced < TREE_COUNT) {
        TraceLog(LOG_WARNING, "Solo se colocaron %d árboles de %d posibles", treesPlaced, TREE_COUNT);
    }

    return treesPlaced;
}

// Dibuja árboles como cilindro rojo + esfera amarilla
void DrawForest(Vector3 *positions, int count) {
    for (int i = 0; i < count; i++) {
        Vector3 pos = positions[i];

        float trunkHeight = 10.0f;
        float trunkRadius = 1.0f;
        float crownHeight = 6.0f;
        float crownRadius = 4.0f;

        Vector3 trunkPos = {
            pos.x,
            pos.y + trunkHeight / 2.0f,
            pos.z
        };

        Vector3 crownPos = {
            pos.x,
            pos.y + trunkHeight + crownHeight / 2.0f,
            pos.z
        };

        DrawCylinder(trunkPos, trunkRadius, trunkRadius, trunkHeight, 16, RED);
        DrawSphere(crownPos, crownRadius, YELLOW);
    }
}
