#include "raylib.h"
#include "terrain.h"
#include "forest.h"
#include "config.h"
#include "water.h"
#include <iostream>

int main(void) {
    InitWindow(1200, 800, "Terreno Procedural");
    SetTargetFPS(60);

    // Inicializar cámara 3D
    Camera3D camera = { 0 };
    float worldSize = TERRAIN_SIZE * TERRAIN_SCALE;
    Vector3 center = { -worldSize / 2, 0, -worldSize / 2 };

    camera.position = Vector3Add(center, (Vector3){ 300, 200, 400 });
    camera.target = (Vector3){ 0, 0, 0 };
    camera.up = (Vector3){ 0, 1, 0 };
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Generar heightmap y árboles
    Image heightmap = GenerateBlendedHeightmap();

    Vector3 treePositions[TREE_COUNT];
    int actualTreeCount = GenerateForest(treePositions, heightmap);
    std::cout << "Árboles colocados: " << actualTreeCount << std::endl;

    // Generar terreno y agua
    Model terrain_model = GenerateTerrain(heightmap);
    SetupTerrainShaderPassiveParameters(&terrain_model.materials[0].shader);

    Model water_model = GenWaterModel();
    SetupWaterShaderPassiveParameters(&water_model.materials[0].shader);

    // Ya no se necesita el heightmap como imagen
    UnloadImage(heightmap);

    float time = 0.0f;

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FREE);
        time += GetFrameTime();

        // Actualizar shaders
        SetupWaterShaderTime(&water_model.materials[0].shader, time);

        float angle = time * 0.2f;
        Vector3 lightDir = {
            cosf(angle),
            -sinf(angle),
            -0.5f
        };

        SetupWaterShaderLight(&water_model.materials[0].shader, lightDir);
        SetupTerrainShaderLight(&terrain_model.materials[0].shader, lightDir);

        // Render
        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);

        SetupTerrainShaderActiveParameters(&terrain_model.materials[0].shader);

        DrawModel(terrain_model, (Vector3){ -worldSize / 2, 0, -worldSize / 2 }, TERRAIN_SCALE, WHITE);
        DrawModel(water_model, (Vector3){ -worldSize / 2, 39.8f, -worldSize / 2 }, TERRAIN_SCALE, WHITE);

        // Dibujar árboles
        DrawForest(treePositions, actualTreeCount);

        EndMode3D();
        EndDrawing();
    }

    // Liberar recursos
    UnloadWaterResources(&water_model);
    UnloadTerrainResources(&terrain_model);
    CloseWindow();

    return 0;
}
