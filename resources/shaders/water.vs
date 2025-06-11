#version 330

// Vertex inputs
in vec3 vertexPosition;
in vec2 vertexTexCoord;

// Outputs to fragment shader
out vec2 TexCoord;
out vec3 FragPos;
out vec3 WorldPos;

// Uniforms from Raylib
uniform float time;
uniform float freqX;
uniform float freqY;
uniform float ampX;
uniform float ampY;
uniform float speedX;
uniform float speedY;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;
uniform float uvScale = 1.0;



void main()
{
    vec3 pos = vertexPosition;

    // Wave distortion on Y (height)
    pos.y += sin(pos.x * freqX + time * speedX) * ampX;
    pos.y += cos(pos.z * freqY + time * speedY) * ampY;

    vec4 worldPosition = matModel * vec4(pos, 1.0);
    FragPos = worldPosition.xyz;
    WorldPos = worldPosition.xyz;
    TexCoord = vertexTexCoord * uvScale;

    gl_Position = matProjection * matView * worldPosition;
}
