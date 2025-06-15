#include "raylib.h"
#include "utils.h"
#include "terrain.h"
#include "vegetation.h"
#include "config.h"
#include "water.h"
#include "player.h"
#include <iostream>

int main(void) {
    InitWindow(1200, 800, "Terreno Procedural");
    SetTargetFPS(60);
    DisableCursor();

    Model water_model = GenWaterModel();
    SetupWaterShaderPassiveParameters(&water_model.materials[0].shader);
    
    Shader shader = LoadShader("resources/shaders/lighting.vs", "resources/shaders/lighting.fs");

    Model tree_model = SetupTreeModel();

    // Crear jugador
    Player player((Vector3){ 0.0f, WORLD_SIZE / 4, 0.0f });

    // Configurar cámara inicial
    Camera3D camera = { 0 };
    camera.up = (Vector3){ 0, 1, 0 };
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    std::map<std::pair<int, int>, Chunk> chunks;

    for(int i = -3; i <= 3; ++i) {
      for(int j = -3; j <= 3; ++j) {
        chunks.emplace(std::make_pair(i, j), Chunk(i,j));
      }
    }

    for(auto& entry : chunks)  entry.second.LoadChunk(&shader);
    
    float time = 0.0f;

    // Bucle principal
    while (!WindowShouldClose()) {

        time += GetFrameTime();

        // Actualizar shaders
        SetupTreeShaderTime(&tree_model.materials[1].shader, time);
        SetupWaterShaderTime(&water_model.materials[0].shader, time);

        // Luz rotatoria
        float angle = time * 0.2f;
        Vector3 lightDir = {
          cosf(angle),
          -sinf(angle),
          -0.5f
        };

        SetupTreeShaderLight(&tree_model.materials[1].shader, lightDir);
        
        int playerChunkX = AbsolutePos2Grid(camera.target.x);
        int playerChunkZ = AbsolutePos2Grid(camera.target.z);

        // Rastrear area visible
        std::unordered_set<std::pair<int, int>, pair_hash> currentVisibleChunks;
        auto visiblePositions = GetChunksAroundPlayer(playerChunkX, playerChunkZ);

        // Generar nuevos chunks y rastrear los visibles
        for (auto& pos : visiblePositions) {
          currentVisibleChunks.insert(pos);

          if (chunks.find(pos) == chunks.end()) {
            Chunk newChunk(pos.first, pos.second);
            newChunk.LoadChunk(&shader);
            chunks.emplace(pos, std::move(newChunk));
          }
        }

        // Descargar los chunks lejanos
        for (auto it = chunks.begin(); it != chunks.end(); ) {
          if (currentVisibleChunks.find(it->first) == currentVisibleChunks.end()) {
            it->second.UnloadChunk();
            it = chunks.erase(it);
          } else {
            ++it;
          }
        }
        
        player.Update(camera, chunks[{playerChunkX, playerChunkZ}].chunk_model);


        // Renderizado
        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);

        player.Draw();
        
        SetupWaterShaderLight(&water_model.materials[0].shader, lightDir);
        
        for(auto& entry : chunks) {
          entry.second.UpdateChunkShader(&shader, lightDir);
          entry.second.DrawChunk(tree_model);
        }

        BeginBlendMode(BLEND_ALPHA);
        for(auto& entry : chunks) {
          int x = std::get<0>(entry.first);
          int z = std::get<1>(entry.first);
          Vector3 water_pos = {
            TERRAIN_SIZE * TERRAIN_SCALE * ((float)x + 0.5f),
            MAX_TERRAIN_HEIGHT * TERRAIN_SCALE / 10.0f ,
            TERRAIN_SIZE * TERRAIN_SCALE * ((float)z + 0.5f)
          };
          DrawModel(water_model, water_pos, TERRAIN_SCALE, WHITE);
        }
        EndBlendMode();
        
        EndMode3D();

        DrawFPS(10, 10);
        DrawText(TextFormat("Player position: (%06.3f, %06.3f, %06.3f)",
                            camera.target.x, camera.target.y, camera.target.z), 10, 30, 20, BLUE);
        DrawText(TextFormat("Grid position: (%03i, %03i)",
                            AbsolutePos2Grid(camera.target.x), AbsolutePos2Grid(camera.target.z)), 10, 50, 20, BLUE);

        EndDrawing();
    }

    // Limpieza
    UnloadWaterResources(&water_model);
    // UnloadTerrainResources(&terrain_model);
    UnloadTreeResources(&tree_model);
    // UnloadImage(heightmap);
    UnloadShader(shader);
    CloseWindow();

    return 0;
}
