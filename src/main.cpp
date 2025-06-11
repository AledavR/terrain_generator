#include "raylib.h"
#include "terrain.h"
#include "forest.h"
#include "config.h"
int main(void) {

  /* Parametros de ventana */
  InitWindow(1200, 800, "Terreno Procedural");
  SetTargetFPS(60);

  /* Se crea la variable para almacenar el heightmap del terreno */
  Image heightmap;
  Model terrain = GenerateTerrain(&heightmap);

  //* Se definen los shaders del terreno */
  Shader shader = LoadShader("lighting.vs", "lighting.fs");
  terrain.materials[0].shader = shader;

  /* Vector direccion de la luz para ser usado con el shaders */
  Vector3 lightDir = Vector3Normalize((Vector3){ -1.0f, -1.0f, -1.0f });
  SetShaderValue(shader, GetShaderLocation(shader, "lightDirection"), &lightDir, SHADER_UNIFORM_VEC3);

  Vector4 col = (Vector4){ 1.0f, 0.8f, 0.6f, 1.0f };
  SetShaderValue(shader, GetShaderLocation(shader, "colDiffuse"), &col, SHADER_UNIFORM_VEC4);

  /* Parametros de camara */
  Camera3D camera = { 0 };
  float worldSize = TERRAIN_SIZE * TERRAIN_SCALE ;
  Vector3 center = { worldSize / 2, 0, worldSize / 2 };
  camera.position = Vector3Add(center, (Vector3){ 80, 130, 150 });
  camera.target = center;
  camera.up = (Vector3){ 0, 1, 0 };
  camera.fovy = 45;
  camera.projection = CAMERA_PERSPECTIVE;
  //Generacion de arboles 
  Vector3 treePositions[TREE_COUNT];
  GenerateForest(treePositions, heightmap);
  //Liberacion
  UnloadImage(heightmap);

  /* Logica principal del programa */
  while (!WindowShouldClose()) {
    UpdateCamera(&camera, CAMERA_ORBITAL);

    BeginDrawing();
    ClearBackground(SKYBLUE);
    BeginMode3D(camera);

    Matrix modelMat = MatrixIdentity();
    SetShaderValueMatrix(shader, GetShaderLocation(shader, "matModel"), modelMat);

    DrawModel(terrain, (Vector3){ 0, 0, 0 }, TERRAIN_SCALE, DARKGREEN);
    DrawModel(terrain, (Vector3){ 0, 0, TERRAIN_SIZE }, TERRAIN_SCALE, DARKGREEN);

    //AQUI DIBUJE LOS ARBOLES
    DrawForest(treePositions);
    DrawGrid(50, 5);
    EndMode3D();
    EndDrawing();
}

  /* Liberación de memoria */
  UnloadModel(terrain);
  CloseWindow();
  return 0;
}