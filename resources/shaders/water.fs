#version 330

in vec2 TexCoord;
in vec3 FragPos, WorldPos;

uniform sampler2D texture0;
uniform vec3 lightDir, moonDir, moonColor, cameraPos;
uniform float time;

out vec4 FragColor;

void main()
{
    vec3 texColor = texture(texture0, TexCoord).rgb;

    vec3 normal = normalize(vec3(sin(WorldPos.z * 2.0 + time) * 0.1, 1.0, cos(WorldPos.x * 2.0 + time) * 0.1));

    vec3 light = normalize(-lightDir);
    vec3 moon  = normalize(-moonDir);
    vec3 viewDir = normalize(cameraPos - FragPos);

    float sunDiffuse  = max(dot(normal, light), 0.0);
    float moonDiffuse = max(dot(normal, moon), 0.0);

    float spec = pow(max(dot(viewDir, reflect(-light, normal)), 0.0), 32.0);

    vec3 color = texColor * (sunDiffuse + moonDiffuse * moonColor) + vec3(0.8) * spec;

    FragColor = vec4(color, 0.55);
}
