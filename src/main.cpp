#include "terrain.h"
#include "water.h"
#include <iostream>

int main(void) {

  /* Parametros de ventana */
  InitWindow(1200, 800, "Terreno Procedural");
  SetTargetFPS(60);

  /* Parametros de camara */
  Camera3D camera = { 0 };
  float worldSize = TERRAIN_SIZE * TERRAIN_SCALE ;
  Vector3 center = { - worldSize / 2, 0, - worldSize / 2};
  camera.position = Vector3Add(center, (Vector3){ 300, 200, 400 });
  camera.target = (Vector3){0, 0, 0} ;
  camera.up = (Vector3){ 0, 1, 0 };
  camera.fovy = 90;
  camera.projection = CAMERA_PERSPECTIVE;

  /* Vector direccion de la luz para ser usado con el shader */
  Vector3 lightDir = Vector3Normalize((Vector3){ -1.0f, -1.0f, -1.0f });
  
  Model terrain_model = GenerateTerrain();
  SetupWaterShaderPassiveParameters(&terrain_model.materials[0].shader);
  
  Model water_model = GenWaterModel();
  SetupWaterShaderPassiveParameters(&water_model.materials[0].shader);

  float time = 0.0f;
  
  /* Logica principal del programa */
  while (!WindowShouldClose()) {
    UpdateCamera(&camera, CAMERA_FREE);

    time += GetFrameTime();
    SetupWaterShaderTime(&water_model.materials[0].shader, time);

    // Simulate the sun moving in a circular arc
    float angle = time * 0.2f;  // Speed of rotation (adjust as needed)
    Vector3 lightDir = {
      cosf(angle),  // X
      -sinf(angle), // Y (negative Y so the light rises and falls)
      -0.5f         // Z (fixed, or vary for more realism)
    };
    
    SetupWaterShaderLight(&water_model.materials[0].shader, lightDir);
    SetupTerrainShaderLight(&terrain_model.materials[0].shader,lightDir);
  
    BeginDrawing();
    ClearBackground(SKYBLUE);
    
    BeginMode3D(camera);

    SetupTerrainShaderActiveParameters(&terrain_model.materials[0].shader);
    // DrawGrid(TERRAIN_SIZE, 5);

    DrawModel(terrain_model, (Vector3){ - worldSize / 2, 0, - worldSize / 2}, TERRAIN_SCALE, WHITE);
    DrawModel(water_model, (Vector3){ - worldSize / 2, 39.8f , - worldSize / 2}, TERRAIN_SCALE, WHITE);
    
    DrawCube( (Vector3){0, 100, 0}, 1.5f, 1.5f, 1.5f, BLUE);

    EndMode3D();

    EndDrawing();
  }

  /* Liberación de memoria */
  UnloadWaterResources(&water_model);
  UnloadTerrainResources(&terrain_model);
  CloseWindow();
  return 0;
}
