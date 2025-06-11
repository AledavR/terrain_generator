#include "terrain.h"

Image GenerateBlendedHeightmap(void) {
  
    /* Genera el terreno base */
  Image base = GenImagePerlinNoise(TERRAIN_SIZE, TERRAIN_SIZE, 0, 0, 3.0f);
    Color *basePixels = LoadImageColors(base);

    /* Genera un terreno auxiliar */
    Image height_layer = GenImagePerlinNoise(TERRAIN_SIZE, TERRAIN_SIZE, 0, 0, 4.0f);
    Color *heightPixels = LoadImageColors(height_layer);

    /* Se crea una variable para almacenar los valores */
    Color *finalPixels = (Color *)RL_MALLOC(TERRAIN_SIZE * TERRAIN_SIZE * sizeof(Color));

    /* Se combinan ambos mapas de ruido */
    for (int i = 0; i < TERRAIN_SIZE * TERRAIN_SIZE; i++) {
        float baseHeight = basePixels[i].r / 255.0f;
        float factor = heightPixels[i].r / 255.0f;

        float finalValue = baseHeight * factor;
        finalValue = fminf(fmaxf(finalValue, 0.0f), 1.0f); // clamp [0,1]

        unsigned char gray = (unsigned char)(finalValue * 255);
        finalPixels[i] = (Color){ gray, gray, gray, 255 };
    }

    /* Se crea la imagen final con los datos de los mapas fusionados */
    Image finalImage = {
        .data = finalPixels,
        .width = TERRAIN_SIZE,
        .height = TERRAIN_SIZE,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };

    /* Limpieza de memoria */
    UnloadImageColors(basePixels);
    UnloadImageColors(heightPixels);
    UnloadImage(base);
    UnloadImage(height_layer);

    return finalImage;
}

/* Se obtiene la altura segun el heightmap generado */
float GetHeightAtPoint(Image heightmap, int x, int z, float maxHeight) {
  if (x < 0) x = 0;
    if (z < 0) z = 0;
    if (x >= heightmap.width) x = heightmap.width - 1;
    if (z >= heightmap.height) z = heightmap.height - 1;

    Color pixel = GetImageColor(heightmap, x, z); /* Lee color en (x,z) */
    
    float heightValue = pixel.r / 255.0f;

    return heightValue * maxHeight;
}

Texture GenerateTerrainTexture(Image heightmap)
{
    // Copy image so we don’t modify the original
    Image map = ImageCopy(heightmap);

    Color *pixels = LoadImageColors(map);

    for (int i = 0; i < map.width * map.height; i++) {
        float value = pixels[i].r / 255.0f;
        if (value < 0.08f) {
          pixels[i] = (Color){ 0, 0, 50, 255 }; // Water
          // pixels[i] = WHITE;
        }
        else if (value < 0.105f) {
          // pixels[i] = WHITE;
          pixels[i] = (Color){ 0, 0, 200, 255 }; // Water
        }
        else if (value < 0.11f) {
          pixels[i] = (Color){ 200, 180, 100, 255 }; // Sandy midland
          // pixels[i] = (Color){ 70, 120, 50, 255 }; // Greenish lowland
        }
        else if (value < 0.3f) {
          pixels[i] = (Color){ 70, 120, 50, 255 }; // Greenish lowland
        }
        else if (value < 0.5f) {
          // pixels[i] = (Color){ 200, 180, 100, 255 }; // Sandy midland
          pixels[i] = BROWN;
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

    return texture;
}

/* Se genera el modelo a partir del heightmap */
Model GenerateTerrain(void) {
  Image heightmap =   GenerateBlendedHeightmap();
  Shader shader = LoadShader("resources/shaders/lighting.vs", "resources/shaders/lighting.fs");
  Mesh mesh = GenMeshHeightmap(heightmap, (Vector3){ TERRAIN_SIZE, MAX_TERRAIN_HEIGHT, TERRAIN_SIZE });
  Model terrain_model = LoadModelFromMesh(mesh);

  /* Se definen los shaders del terreno */
 terrain_model.materials[0].shader = shader;
 terrain_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = GenerateTerrainTexture(heightmap);

 /* Liberacion de espacio en memoria (El terreno ya fue generado para este punto) */
 UnloadImage(heightmap);
 
 return terrain_model;
}

void SetupTerrainShaderPassiveParameters(Shader* terrain_shader)
{
  Vector4 color = (Vector4){1.0f, 0.8f, 0.6f, 1.0f};
  SetShaderValue(*terrain_shader, GetShaderLocation(*terrain_shader, "colDiffuse"), &color, SHADER_UNIFORM_VEC4);
}

void SetupTerrainShaderLight(Shader* terrain_shader, Vector3 light_dir)
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
