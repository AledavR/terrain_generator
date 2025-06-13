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
    
    Player player((Vector3){ 0.0f, WORLD_SIZE / 10, 0.0f });
    
    Camera camera = { 0 };
    camera.position = Vector3Add(player.position, (Vector3){ 0.0f, 3.0f, -6.0f });  // Above and behind
    camera.target = player.position;
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    // // Inicializar cámara 3D
    // Camera3D camera = { 0 };
    float worldSize = TERRAIN_SIZE * TERRAIN_SCALE;
    Vector3 center = { -worldSize / 2, 0, -worldSize / 2 };

    // camera.position = Vector3Add(center, (Vector3){ 300, 200, 400 });
    // camera.target = (Vector3){ 0, 0, 0 };
    // camera.up = (Vector3){ 0, 1, 0 };
    // camera.fovy = 90.0f;
    // camera.projection = CAMERA_PERSPECTIVE;

    // Generar heightmap y árboles
    Image heightmap = GenerateBlendedHeightmap();

    Vector3 treePositions[TREE_COUNT];
    int actualTreeCount = GenerateForest(treePositions, heightmap);
    std::cout << "Árboles colocados: " << actualTreeCount << std::endl;

    // Generar terreno y agua
    Model terrain_model = GenerateTerrain(heightmap);
    SetupTerrainShaderPassiveParameters(&terrain_model.materials[0].shader);
    Model collision_model = GenerateCollisionModel(heightmap, terrain_model.transform);

    Model water_model = GenWaterModel();
    SetupWaterShaderPassiveParameters(&water_model.materials[0].shader);

    Model tree_model = SetupTreeModel();

    // Ya no se necesita el heightmap como imagen
    UnloadImage(heightmap);

    float time = 0.0f;

    while (!WindowShouldClose()) {

      
      HideCursor();
      // Define once at top or config
      float mouseBoundaryRadius = 100.0f;  // Radius in pixels

      // Get screen center and mouse position
      Vector2 center = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
      Vector2 mouse = GetMousePosition();

      // Compute offset vector from center
      Vector2 offset = Vector2Subtract(mouse, center);
      float dist = Vector2Length(offset);

      if (dist > mouseBoundaryRadius) {
        // Normalize direction and reflect to opposite side
        Vector2 direction = Vector2Normalize(offset);
        Vector2 reflected = Vector2Scale(direction, -mouseBoundaryRadius);
        Vector2 newMousePos = Vector2Add(center, reflected);
        SetMousePosition(newMousePos.x, newMousePos.y);
      }

      
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        time += GetFrameTime();

        player.Update(camera, collision_model);

        // Actualizar shaders
        SetupWaterShaderTime(&water_model.materials[0].shader, time);
        SetupTreeShaderTime(&tree_model.materials[1].shader, time);

        float angle = time * 0.2f;
        Vector3 lightDir = {
            cosf(angle),
            -sinf(angle),
            -0.5f
        };

        SetupWaterShaderLight(&water_model.materials[0].shader, lightDir);
        SetupTerrainShaderLight(&terrain_model.materials[0].shader, lightDir);
        SetupTreeShaderLight(&tree_model.materials[1].shader, lightDir);
        
        // Render
        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);
        

        SetupTerrainShaderActiveParameters(&terrain_model.materials[0].shader);
        // SetupTreeShaderActiveParameters(&tree_model.materials[1].shader);

        // DrawModel(terrain_model, (Vector3){ -worldSize / 2, 0, -worldSize / 2 }, TERRAIN_SCALE, WHITE);
        DrawModel(terrain_model, (Vector3){ 0,0,0 }, 1.0f, WHITE);
        // DrawModel(water_model, (Vector3){ -worldSize / 2, 0.0f, -worldSize / 2 }, TERRAIN_SCALE, WHITE);
        DrawModel(water_model, (Vector3){ 0 , WORLD_SIZE / 40, 0}, TERRAIN_SCALE, WHITE);
        
        player.Draw();

        // Dibujar árboles
        DrawForest(treePositions, actualTreeCount, tree_model);

        EndMode3D();

        DrawFPS(10, 10);
        
        EndDrawing();
    }

    // Liberar recursos
    UnloadWaterResources(&water_model);
    UnloadTerrainResources(&terrain_model);
    UnloadTreeResources(&terrain_model);
    CloseWindow();

    return 0;
}
