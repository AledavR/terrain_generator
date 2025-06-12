#version 330

in vec3 fragNormal;
in vec2 fragTexCoord;

uniform vec3 lightDirection;
uniform vec4 colDiffuse;
uniform sampler2D texture0;
uniform vec3 moonDirection;
uniform vec3 moonColor;

out vec4 finalColor;

void main() {
  vec3 n = normalize(fragNormal);

  // --- Sun light ---
  float sunDiffuse = max(dot(n, -lightDirection), 0.0);

  // --- Moon light ---
  float moonDiffuse = max(dot(n, -moonDirection), 0.0);

  // --- Texture color ---
  vec4 texColor = texture(texture0, fragTexCoord);

  // --- Combined lighting ---
  vec3 color = texColor.rgb * colDiffuse.rgb * (sunDiffuse + moonDiffuse * moonColor);
  float alpha = texColor.a * colDiffuse.a;

  finalColor = vec4(color, alpha);
}
