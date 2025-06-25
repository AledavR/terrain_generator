#version 330

in vec3 vertexPosition, vertexNormal;
in vec2 vertexTexCoord;

uniform mat4 mvp, matModel;

out vec3 fragNormal;
out vec2 fragTexCoord;

void main() {
  fragNormal = mat3(transpose(inverse(matModel))) * vertexNormal;
  fragTexCoord = vertexTexCoord;
  gl_Position = mvp * vec4(vertexPosition, 1.0);
}
