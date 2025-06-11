#include "water.h"

Model GenWaterModel(void)
{
  // Load a water texture (replace "resources/water.png" with your actual path)
  Texture2D waterTexture = LoadTexture("resources/textures/water_texture.jpg");
  Model water_model = LoadModelFromMesh(GenMeshPlane(WORLD_SIZE , WORLD_SIZE, 50, 50));
  Shader water_shader = LoadShader("resources/shaders/water.vs", "resources/shaders/water.fs");
  
  // water.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = (Color){0, 100, 255, 100};
  water_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = waterTexture;
  water_model.materials[0].shader = water_shader;

  return water_model;
}

void SetupWaterShaderPassiveParameters(Shader *water_shader)
{
  float freqX = 2.0f, freqY = 2.0f;
  float ampX = 0.6f, ampY = 0.6f;
  float speedX = 1.2f, speedY = 1.2f;

  float uvScale = 150.0f;
  SetShaderValue(*water_shader, GetShaderLocation(*water_shader, "uvScale"), &uvScale, SHADER_UNIFORM_FLOAT);
  SetShaderValue(*water_shader, GetShaderLocation(*water_shader, "freqX"), &freqX, SHADER_UNIFORM_FLOAT);
  SetShaderValue(*water_shader, GetShaderLocation(*water_shader, "freqY"), &freqY, SHADER_UNIFORM_FLOAT);
  SetShaderValue(*water_shader, GetShaderLocation(*water_shader, "ampX"), &ampX, SHADER_UNIFORM_FLOAT);
  SetShaderValue(*water_shader, GetShaderLocation(*water_shader, "ampY"), &ampY, SHADER_UNIFORM_FLOAT);
  SetShaderValue(*water_shader, GetShaderLocation(*water_shader, "speedX"), &speedX, SHADER_UNIFORM_FLOAT);
  SetShaderValue(*water_shader, GetShaderLocation(*water_shader, "speedY"), &speedY, SHADER_UNIFORM_FLOAT);
}

void SetupWaterShaderLight(Shader *water_shader, Vector3 light_dir)
{
  SetShaderValue(*water_shader, GetShaderLocation(*water_shader, "lightDir"), &light_dir, SHADER_UNIFORM_VEC3);
}

void SetupWaterShaderTime(Shader *water_shader, float time)
{
  SetShaderValue(*water_shader, GetShaderLocation(*water_shader, "time"), &time, SHADER_UNIFORM_FLOAT);
}

void UnloadWaterResources(Model* water_model)
{
    UnloadTexture(water_model->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
    UnloadShader(water_model->materials[0].shader);
    UnloadModel(*water_model);
}
