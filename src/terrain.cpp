#include "terrain.h"

Image GenerateBlendedHeightmap(void) {
  
    /* Genera el terreno base */
    Image base = GenImagePerlinNoise(TERRAIN_SIZE, TERRAIN_SIZE, 0, 0, 1.0f);
    Color *basePixels = LoadImageColors(base);

    /* Genera un terreno auxiliar */
    Image height_layer = GenImagePerlinNoise(TERRAIN_SIZE, TERRAIN_SIZE, 0, 0, 0.6f);
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

/* Se genera el modelo a partir del heightmap */
Model GenerateTerrain(Image* heightmapOut) {
  *heightmapOut =   GenerateBlendedHeightmap();

  Mesh mesh = GenMeshHeightmap(*heightmapOut, (Vector3){ TERRAIN_SIZE, MAX_TERRAIN_HEIGHT, TERRAIN_SIZE });
  return LoadModelFromMesh(mesh);
}