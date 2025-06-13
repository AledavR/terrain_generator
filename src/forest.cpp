#include "forest.h"
#include <stdlib.h>
#include <math.h>

// Genera posiciones válidas para árboles flotantes
int GenerateForest(Vector3 *positions, Image heightmap) {
    int treesPlaced = 0;
    int maxAttempts = TREE_COUNT * 5;
    int attempts = 0;

    Vector3 offset = { -WORLD_SIZE / 2, 0, -WORLD_SIZE / 2 };

    while (treesPlaced < TREE_COUNT && attempts < maxAttempts) {
        int x = rand() % TERRAIN_SIZE;
        int z = rand() % TERRAIN_SIZE;

        float height = GetHeightAtPoint(heightmap, x, z);

        // Solo colocar árboles entre cierta altitud
        if (height >= 0.11f && height <= 0.3f) {
            positions[treesPlaced++] = (Vector3){
                x * TERRAIN_SCALE + offset.x,
                height * MAX_TERRAIN_HEIGHT * TERRAIN_SCALE + 1.0f,
                z * TERRAIN_SCALE + offset.z
            };
        }

        attempts++;
    }

    return treesPlaced;
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

void DrawForest(Vector3 *positions, int count, Model tree_model) {
    for (int i = 0; i < count; i++) {
        DrawModel(tree_model, positions[i], 4.0f, WHITE);
    }
}
