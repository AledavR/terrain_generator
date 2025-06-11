#ifndef FOREST_H
#define FOREST_H

#include "raylib.h"
#include "config.h"
#include "terrain.h"

// Genera posiciones de árboles válidas
void GenerateForest(Vector3 *positions, Image heightmap);

// Dibuja árboles en pantalla
void DrawForest(Vector3 *positions);

#endif // FOREST_H