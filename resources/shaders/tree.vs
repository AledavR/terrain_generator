#version 330

in vec3 vertexPosition, vertexNormal;
in vec2 vertexTexCoord;

out vec3 fragNormal;
out vec2 fragTexCoord;

uniform float time, freqX, freqZ, speedX, speedZ;
uniform mat4 mvp, matModel, matProjection;

void main() {
  vec3 pos = vertexPosition;

  float waveX = sin(pos.x * freqX + time * speedX);
  float swayStrength = clamp(pos.y, 0.0, 1.0);
  pos.x += waveX * swayStrength * 0.2;

  fragNormal = mat3(transpose(inverse(matModel))) * vertexNormal; // Transform normal
  fragTexCoord = vertexTexCoord;

  gl_Position = mvp * vec4(pos, 1.0);
}
