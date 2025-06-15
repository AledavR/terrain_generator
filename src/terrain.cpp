#include "terrain.h"
#include "vegetation.h"
#include "utils.h"
#include <raylib.h>

void Chunk::GenerateChunkTexture()
{
  // Copy image so we don’t modify the original
  Image map = ImageCopy(chunk_heightmap);
  Color *pixels = LoadImageColors(map);

  for (int i = 0; i < map.width * map.height; i++) {
    float value = pixels[i].r / 255.0f;
    if (value < 0.08f) {
      pixels[i] = (Color){ 0, 0, 50, 255 }; // Deep water
    }
    else if (value < 0.105f) {
      pixels[i] = (Color){ 0, 0, 200, 255 }; // Water
    }
    else if (value < 0.11f) {
      pixels[i] = (Color){ 200, 180, 100, 255 }; // Sandy midland
    }
    else if (value < 0.3f) {
      pixels[i] = (Color){ 70, 120, 50, 255 }; // Greenish lowland
    }
    else if (value < 0.5f) {
      pixels[i] = BROWN; // Mountains
    }
    else {
      pixels[i] = (Color){ 200, 200, 200, 255 }; // Rocky or snowy highland
    }
  }
  
  // Update map data with modified colors
  Image updatedImage = {
    .data = pixels,
    .width = map.width,
    .height = map.height,
    .mipmaps = 1,
    .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
  };

  Texture2D texture = LoadTextureFromImage(updatedImage);

  // Free color memory AFTER image->texture conversion
  UnloadImageColors(pixels);
  UnloadImage(map);
  // UnloadImage(updatedImage);

  chunk_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

void Chunk::GenerateChunkMesh(Shader* terrain_shader)
{
  Mesh mesh = GenMeshHeightmap(chunk_heightmap,
                               (Vector3){ TERRAIN_SIZE, MAX_TERRAIN_HEIGHT, TERRAIN_SIZE });
  chunk_model = LoadModelFromMesh(mesh);
  Matrix transform =
    MatrixMultiply(MatrixTranslate(
                                   grid_position[0] * TERRAIN_SIZE,
                                   0,
                                   grid_position[1] * TERRAIN_SIZE
                                  ),
                                    MatrixScale(TERRAIN_SCALE, TERRAIN_SCALE, TERRAIN_SCALE));
  chunk_model.transform = transform;
  chunk_model.materials[0].shader = *terrain_shader;
}

void Chunk::LoadChunk(Shader* terrain_shader)
{
  GenerateChunkMesh(terrain_shader);
  GenerateChunkTexture();

  tree_count = GenerateChunkVegetation(tree_positions, *this);
  
  is_loaded = true;
}

void Chunk::UnloadChunk(void)
{
  if (is_loaded)
    {
      UnloadImage(chunk_heightmap);
      UnloadModel(chunk_model);
      is_loaded = false;
    }
}

void Chunk::DrawChunk(Model tree_model)
{
  DrawModel(chunk_model, (Vector3){0, 0, 0}, 1.0f, WHITE);
  for(int i = 0; i < tree_count; i++) {
    DrawModel(tree_model, tree_positions[i], 4.0f, WHITE);
  }
}

void Chunk::UpdateChunkShader(Shader* terrain_shader, Vector3 light_dir)
{
  SetShaderValue(*terrain_shader,
                 GetShaderLocation(*terrain_shader, "lightDirection"),
                 &light_dir, SHADER_UNIFORM_VEC3);

  Vector3 moonDir = Vector3Negate(light_dir); 
  Vector3 moonColor = {0.2f, 0.3f, 0.5f};

  SetShaderValue(*terrain_shader, GetShaderLocation(*terrain_shader, "lightDirection"), &light_dir, SHADER_UNIFORM_VEC3);
  SetShaderValue(*terrain_shader, GetShaderLocation(*terrain_shader, "moonDirection"), &moonDir, SHADER_UNIFORM_VEC3);
  SetShaderValue(*terrain_shader, GetShaderLocation(*terrain_shader, "moonColor"), &moonColor, SHADER_UNIFORM_VEC3);
  
}

void SetupTerrainShaderPassiveParameters(Shader* terrain_shader)
{
  Vector4 color = (Vector4){1.0f, 0.8f, 0.6f, 1.0f};
  SetShaderValue(*terrain_shader, GetShaderLocation(*terrain_shader, "colDiffuse"), &color, SHADER_UNIFORM_VEC4);
}

 void SetupTerrainShaderLight(Shader* terrain_shader, Vector3 light_dir){

  SetShaderValue(*terrain_shader,
                 GetShaderLocation(*terrain_shader, "lightDirection"),
                 &light_dir, SHADER_UNIFORM_VEC3);

  Vector3 moonDir = Vector3Negate(light_dir); 
  Vector3 moonColor = {0.2f, 0.3f, 0.5f};
  
  SetShaderValue(*terrain_shader, GetShaderLocation(*terrain_shader, "lightDirection"), &light_dir, SHADER_UNIFORM_VEC3);
  SetShaderValue(*terrain_shader, GetShaderLocation(*terrain_shader, "moonDirection"), &moonDir, SHADER_UNIFORM_VEC3);
  SetShaderValue(*terrain_shader, GetShaderLocation(*terrain_shader, "moonColor"), &moonColor, SHADER_UNIFORM_VEC3);

}

void SetupTerrainShaderActiveParameters(Shader* terrain_shader)
{
  SetShaderValueMatrix(*terrain_shader, GetShaderLocation(*terrain_shader, "matModel"), MatrixIdentity());
} 

void UnloadTerrainResources(Model *terrain_model)
{
  UnloadTexture(terrain_model->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
  UnloadShader(terrain_model->materials[0].shader);
  UnloadModel(*terrain_model);
}
