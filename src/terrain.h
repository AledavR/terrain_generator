#ifndef TERRAIN_H
#define TERRAIN_H

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include "config.h"

Image GenerateBlendedHeightmap(void);
float GetHeightAtPoint(Image heightmap, int x, int z);
Texture GenerateTerrainTexture(Image heightmap);
Model GenerateTerrain(Image heightmap);
void SetupTerrainShaderPassiveParameters(Shader* terrain_shader);
void SetupTerrainShaderLight(Shader* terrain_shader, Vector3 light_dir);
void SetupTerrainShaderActiveParameters(Shader* terrain_shader);
void UnloadTerrainResources(Model *terrain_model);

#endif // TERRAIN_H

