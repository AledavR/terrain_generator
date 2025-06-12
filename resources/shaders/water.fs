#version 330

in vec2 TexCoord;
in vec3 FragPos;
in vec3 WorldPos;

out vec4 FragColor;

uniform sampler2D texture0;
uniform vec3 lightDir;
uniform vec3 moonDir;
uniform vec3 moonColor;
uniform vec3 cameraPos;
uniform float time;

void main()
{
  vec3 texColor = texture(texture0, TexCoord).rgb;

  // Fake dynamic normal for lighting
  vec3 normal;
  normal.x = sin(WorldPos.z * 2.0 + time) * 0.1;
  normal.y = 1.0;
  normal.z = cos(WorldPos.x * 2.0 + time) * 0.1;
  normal = normalize(normal);

  // Normalize light directions
  vec3 light = normalize(-lightDir);
  vec3 moon = normalize(-moonDir);

  // Diffuse lighting
  float sunDiffuse = max(dot(normal, light), 0.0);
  float moonDiffuse = max(dot(normal, moon), 0.0);

  // Specular from sun only
  vec3 viewDir = normalize(cameraPos - FragPos);
  vec3 reflectDir = reflect(-light, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

  // Final color with sun and moon
  vec3 color = texColor * (sunDiffuse + moonDiffuse * moonColor) + vec3(0.8) * spec;

  FragColor = vec4(color, 0.55);
}
