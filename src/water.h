#ifndef WATER_H
#define WATER_H

#include "raylib.h"
#include "raymath.h"
#include "config.h"

Model GenWaterModel(void);
void SetupWaterShaderPassiveParameters(Shader *water_shader);
void SetupWaterShaderLight(Shader *water_shader, Vector3 light_dir);
void SetupWaterShaderTime(Shader *water_shader, float time);
void UnloadWaterResources(Model* water_model);

#endif // WATER_H
