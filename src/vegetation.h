#ifndef VEGETATION_H
#define VEGETATION_H

#include "raylib.h"
#include "config.h"
#include "utils.h"
#include "terrain.h"
#include <math.h>

// Genera posiciones de árboles válidas
int GenerateForest(Vector3 *positions, Image heightmap);

Model SetupTreeModel(void);
void SetupTreeShaderPassiveParameters(Shader* tree_shader);
void SetupTreeShaderLight(Shader* tree_shader, Vector3 light_dir);
void SetupTreeShaderActiveParameters(Shader* tree_shader);
void SetupTreeShaderTime(Shader* tree_shader, float time);
void UnloadTreeResources(Model *tree_model);
int GenerateChunkVegetation(Vector3* positions, Chunk& chunk);



#endif
