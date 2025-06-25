#version 330

in vec3 fragNormal;
in vec2 fragTexCoord;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec3 lightDirection, moonDirection, moonColor;

out vec4 finalColor;

void main() {
  vec3 n = normalize(fragNormal);

  float sunDiffuse = max(dot(n, -lightDirection), 0.0);
  float moonDiffuse = max(dot(n, -moonDirection), 0.0);

  vec3 lighting = sunDiffuse + moonDiffuse * moonColor;
    
  // Sample the texture and multiply by diffuse color
  vec4 texColor = texture(texture0, fragTexCoord) * colDiffuse;
    
  vec3 finalRgb = texColor.rgb * lighting;
  float finalAlpha = texColor.a;

  finalColor = vec4(finalRgb, finalAlpha);
}
