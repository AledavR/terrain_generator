#include "vegetation.h"
#include "terrain.h"
#include "config.h"
#include "utils.h"
#include <random>
#include <raylib.h>

// Genera posiciones válidas para árboles flotantes
int GenerateChunkVegetation(Vector3* positions, Chunk& chunk)
{
    int trees_placed = 0;

    int chunk_x = chunk.grid_position[0];
    int chunk_z = chunk.grid_position[1];

    Vector3 origin = {
        (chunk_x * TERRAIN_SIZE) * TERRAIN_SCALE,
        0,
        (chunk_z * TERRAIN_SIZE) * TERRAIN_SCALE
    };

    std::mt19937 rng(chunk.seed);
    std::uniform_int_distribution<int> dist_x(0, TERRAIN_SIZE);
    std::uniform_int_distribution<int> dist_z(0, TERRAIN_SIZE);
    std::uniform_real_distribution<float> offset(0.0f, 1.0f);

    int attempts = 0;
    const int max_attempts = TREE_COUNT * 10;

    while (trees_placed < TREE_COUNT && attempts++ < max_attempts) {
        int x = dist_x(rng);
        int z = dist_z(rng);

        float height = GetHeightAtPoint(chunk.chunk_heightmap, x, z);
        if (height < 0.11f || height > 0.3f) continue;

        if (offset(rng) < 0.8f) continue;

        positions[trees_placed++] = (Vector3){
            origin.x + x * TERRAIN_SCALE,
            height * MAX_TERRAIN_HEIGHT * TERRAIN_SCALE,
            origin.z + z * TERRAIN_SCALE
        };
    }

    return trees_placed;
}

Model SetupTreeModel(void) {
    Model tree_model = LoadModel("resources/models/tree.obj");
    Shader tree_shader = LoadShader("resources/shaders/tree.vs", "resources/shaders/tree.fs");
    tree_model.materials[1].shader = tree_shader;

    float freqX = 1.0f, speedX = 0.6f;
    SetShaderValue(tree_shader, GetShaderLocation(tree_shader, "freqX"), &freqX, SHADER_UNIFORM_FLOAT);
    SetShaderValue(tree_shader, GetShaderLocation(tree_shader, "speedX"), &speedX, SHADER_UNIFORM_FLOAT);

    return tree_model;
}

void SetupTreeShaderLight(Shader* tree_shader, Vector3 light_dir) {
    Vector3 moonDir = Vector3Negate(light_dir); 
    Vector3 moonColor = {0.2f, 0.3f, 0.5f};

    SetShaderValue(*tree_shader, GetShaderLocation(*tree_shader, "lightDirection"), &light_dir, SHADER_UNIFORM_VEC3);
    SetShaderValue(*tree_shader, GetShaderLocation(*tree_shader, "moonDirection"), &moonDir, SHADER_UNIFORM_VEC3);
    SetShaderValue(*tree_shader, GetShaderLocation(*tree_shader, "moonColor"), &moonColor, SHADER_UNIFORM_VEC3);
}

void SetupTreeShaderActiveParameters(Shader* tree_shader) {
    SetShaderValueMatrix(*tree_shader, GetShaderLocation(*tree_shader, "matModel"), MatrixIdentity());
}

void SetupTreeShaderTime(Shader* tree_shader, float time) {
    SetShaderValue(*tree_shader, GetShaderLocation(*tree_shader, "time"), &time, SHADER_UNIFORM_FLOAT);
}

void UnloadTreeResources(Model *tree_model) {
    UnloadShader(tree_model->materials[0].shader);
    UnloadModel(*tree_model);
}

