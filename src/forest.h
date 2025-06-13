#ifndef FOREST_H
#define FOREST_H

#include "raylib.h"
#include "config.h"
#include "terrain.h"

// Genera posiciones de árboles válidas
int GenerateForest(Vector3 *positions, Image heightmap);

Model SetupTreeModel(void);
void SetupTreeShaderPassiveParameters(Shader* tree_shader);
void SetupTreeShaderLight(Shader* tree_shader, Vector3 light_dir);
void SetupTreeShaderActiveParameters(Shader* tree_shader);
void SetupTreeShaderTime(Shader* tree_shader, float time);
void UnloadTreeResources(Model *tree_model);

// Dibuja árboles en pantalla
void DrawForest(Vector3 *positions, int count, Model tree_model);

#endif // FOREST_H
