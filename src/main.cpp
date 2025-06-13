#include "raylib.h"
#include "terrain.h"
#include "forest.h"
#include "config.h"
#include "water.h"
#include "player.h"
#include <iostream>

int main(void) {
    InitWindow(1200, 800, "Terreno Procedural");
    SetTargetFPS(60);
    DisableCursor();
    
    // Heightmap y modelos
    Image heightmap = GenerateBlendedHeightmap();
    Model terrain_model = GenerateTerrain(heightmap);
    SetupTerrainShaderPassiveParameters(&terrain_model.materials[0].shader);
    Model collision_model = GenerateCollisionModel(heightmap, terrain_model.transform);

    Model water_model = GenWaterModel();
    SetupWaterShaderPassiveParameters(&water_model.materials[0].shader);

    Model tree_model = SetupTreeModel();

    Vector3 treePositions[TREE_COUNT];
    int actualTreeCount = GenerateForest(treePositions, heightmap);
    std::cout << "Árboles colocados: " << actualTreeCount << std::endl;

    // Crear jugador
    Player player((Vector3){ 0.0f, WORLD_SIZE / 4, 0.0f });

    // Configurar cámara inicial
    Camera3D camera = { 0 };
    camera.up = (Vector3){ 0, 1, 0 };
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float time = 0.0f;

    // Bucle principal
    while (!WindowShouldClose()) {

        time += GetFrameTime();
        player.Update(camera, collision_model);

        // Actualizar shaders
        SetupWaterShaderTime(&water_model.materials[0].shader, time);
        SetupTreeShaderTime(&tree_model.materials[1].shader, time);

        // Luz rotatoria
        float angle = time * 0.2f;
        Vector3 lightDir = {
            cosf(angle),
            -sinf(angle),
            -0.5f
        };

        SetupWaterShaderLight(&water_model.materials[0].shader, lightDir);
        SetupTerrainShaderLight(&terrain_model.materials[0].shader, lightDir);
        SetupTreeShaderLight(&tree_model.materials[1].shader, lightDir);

        // Renderizado
        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);
        

        SetupTerrainShaderActiveParameters(&terrain_model.materials[0].shader);

        DrawModel(terrain_model, (Vector3){ 0,0,0 }, 1.0f, WHITE);
        DrawModel(water_model, (Vector3){ 0 , WORLD_SIZE / 40, 0}, TERRAIN_SCALE, WHITE);

        // Dibujar árboles
        DrawForest(treePositions, actualTreeCount, tree_model);
        player.Draw();

        EndMode3D();

        DrawFPS(10, 10);
        
        EndDrawing();
    }

    // Limpieza
    UnloadWaterResources(&water_model);
    UnloadTerrainResources(&terrain_model);
    UnloadTreeResources(&terrain_model);
    UnloadImage(heightmap);
    CloseWindow();

    return 0;
}
