#ifndef TERRAIN_H
#define TERRAIN_H

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include "config.h"
#include "utils.h"

class Chunk {
 public: 
  int grid_position[2];
  bool is_loaded;
  int seed;
  Image chunk_heightmap;
  Model chunk_model;
  Vector3 tree_positions[TREE_COUNT];
  int tree_count;

  Chunk(){}
  Chunk(int position_x, int position_z, int global_seed) {
    grid_position[0] = position_x;
    grid_position[1] = position_z;
    seed = GenerateChunkSeed(position_x, position_z);
    chunk_heightmap = GenerateBlendedHeightmap(position_x, position_z, global_seed);
  }
  void LoadChunk(Shader* terrain_shader);
  void UnloadChunk(void);
  void DrawChunk(Model tree_model);
  void UpdateChunkShader(Shader* terrain_shader, Vector3 light_dir);

 private:
  void GenerateChunkMesh(Shader* terrain_shader);
  void GenerateChunkTexture(void);
  
};

void SetupTerrainShaderPassiveParameters(Shader* terrain_shader);
void SetupTerrainShaderLight(Shader* terrain_shader, Vector3 light_dir);
void SetupTerrainShaderActiveParameters(Shader* terrain_shader);
void UnloadTerrainResources(Model *terrain_model);

#endif // TERRAIN_H

