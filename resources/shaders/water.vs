#version 330

// Vertex inputs
in vec3 vertexPosition;
in vec2 vertexTexCoord;

// Outputs to fragment shader
out vec2 TexCoord;
out vec3 FragPos, WorldPos;

// Uniforms from Raylib
uniform float time, freqX, freqY, ampX, ampY, speedX, speedY, uvScale = 1.0;

uniform mat4 matModel, matView, matProjection;

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
