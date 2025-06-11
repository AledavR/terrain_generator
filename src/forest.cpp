#include "forest.h"
#include <stdlib.h>
#include <math.h>

// Genera posiciones válidas para árboles
void GenerateForest(Vector3 *positions, Image heightmap) {
    int treesPlaced = 0;
    int maxAttempts = TREE_COUNT * 20;
    int attempts = 0;

    while (treesPlaced < TREE_COUNT && attempts < maxAttempts) {
        int x = rand() % TERRAIN_SIZE;
        int z = rand() % TERRAIN_SIZE;

        float height = GetHeightAtPoint(heightmap, x, z, MAX_TERRAIN_HEIGHT);

        // Solo colocamos árboles en zonas adecuadas de altura
        if (height >= 1.0f && height <= 38.0f) {
            Color pixel = GetImageColor(heightmap, x, z);
            if (pixel.r < 250) {
                positions[treesPlaced++] = (Vector3){
                    x * TERRAIN_SCALE,
                    height,
                    z * TERRAIN_SCALE
                };
            }
        }
        attempts++;
    }

    if (treesPlaced < TREE_COUNT) {
        TraceLog(LOG_WARNING, "Solo se colocaron %d árboles de %d", treesPlaced, TREE_COUNT);
    }
}

// Dibuja árboles tipo voxel (tronco + copa)
void DrawForest(Vector3 *positions) {
    for (int i = 0; i < TREE_COUNT; i++) {
        Vector3 pos = positions[i];

        // Tronco: cubo delgado
        DrawCube(
            (Vector3){ pos.x, pos.y + 1.0f, pos.z },  // centro del tronco
            0.5f, 2.0f, 0.5f,                         // tamaño del cubo
            BROWN
        );

        // Copa: cubo grande encima del tronco
        DrawCube(
            (Vector3){ pos.x, pos.y + 3.0f, pos.z },  // centro de la copa
            2.0f, 2.0f, 2.0f,
            GREEN
        );
    }
}
