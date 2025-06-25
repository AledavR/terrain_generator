#include "utils.h"

int GenerateChunkSeed(int x, int z)
{
    return (x * 73856093) ^ (z * 19349663); // large primes = hash-like
}

Image GenerateBlendedHeightmap(int position_x, int position_z, int seed)
{

 int offsetX = (position_x * TERRAIN_SIZE) + seed;
 int offsetY = (position_z * TERRAIN_SIZE) + seed;
  
  /* Genera el terreno base */
  Image base = GenImagePerlinNoise((TERRAIN_SIZE + 1), (TERRAIN_SIZE + 1), offsetX, offsetY, TERRAIN_SCALE * 0.3f);
  Color *basePixels = LoadImageColors(base);

  /* Genera un terreno auxiliar */
  Image height_layer = GenImagePerlinNoise((TERRAIN_SIZE + 1), (TERRAIN_SIZE + 1), offsetX, offsetY, TERRAIN_SCALE * 0.6f);
  Color *heightPixels = LoadImageColors(height_layer);

  /* Se crea una variable para almacenar los valores */
  Color *finalPixels = (Color *)RL_MALLOC((TERRAIN_SIZE + 1) * (TERRAIN_SIZE + 1) * sizeof(Color));

  /* Se combinan ambos mapas de ruido */
  for (int i = 0; i < (TERRAIN_SIZE + 1) * (TERRAIN_SIZE + 1); i++) {
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
    .width = (TERRAIN_SIZE + 1),
    .height = (TERRAIN_SIZE + 1),
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
float GetHeightAtPoint(Image heightmap, int x, int z)
{
  if (x < 0) x = 0;
    if (z < 0) z = 0;
    if (x >= heightmap.width) x = heightmap.width - 1;
    if (z >= heightmap.height) z = heightmap.height - 1;

    Color pixel = GetImageColor(heightmap, x, z); /* Lee color en (x,z) */
    float heightValue = pixel.r / 255.0f;

    return heightValue;
}

int AbsolutePos2Grid(float l)
{
  return floor(l / TERRAIN_SIZE / TERRAIN_SCALE);
}

std::vector<std::pair<int, int>> GetChunksAroundPlayer(int centerX, int centerZ) {
    std::vector<std::pair<int, int>> result;
    for (int i = -CHUNK_RADIUS; i <= CHUNK_RADIUS; ++i) {
        for (int j = -CHUNK_RADIUS; j <= CHUNK_RADIUS; ++j) {
            result.emplace_back(centerX + i, centerZ + j);
        }
    }
    return result;
}



void DrawHelpMenu()
{
    Color transparentGreen = (Color){0, 255, 0, 150};
    Color borderColor = DARKGREEN;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int menuX = 50;
    int menuY = 50;
    int menuWidth = screenWidth - 100;
    int menuHeight = screenHeight - 100;

    int borderThickness = 4;

    DrawRectangle(menuX - borderThickness, menuY - borderThickness,
                  menuWidth + 2 * borderThickness, menuHeight + 2 * borderThickness, borderColor);

    DrawRectangle(menuX, menuY, menuWidth, menuHeight, transparentGreen);

    int textX = menuX + 20;
    int textY = menuY + 30;
    int lineHeight = 25;

    DrawText("Controles", textX, textY, 30, DARKGREEN);
    textY += 50;

    DrawText("- W / A / S / D: Mover usuario", textX, textY, 20, BLACK); textY += lineHeight;
    DrawText("- Mouse: Mover camara", textX, textY, 20, BLACK); textY += lineHeight;
    DrawText("- Rueda del Mouse: Acercar o alejar camara", textX, textY, 20, BLACK); textY += lineHeight;
    DrawText("- Space: Saltar", textX, textY, 20, BLACK); textY += lineHeight;
    DrawText("- Escape: Cerrar el programa", textX, textY, 20, BLACK); textY += lineHeight;
    DrawText("- H: Activar/Desactivar menu de ayuda", textX, textY, 20, BLACK); textY += lineHeight;
}
