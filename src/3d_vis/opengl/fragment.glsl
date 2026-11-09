#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 TrackColor;

out vec4 FragColor;

uniform float ambientIntensity;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 ambient = ambientIntensity * TrackColor;
    vec3 diffuse = diff * TrackColor;

    FragColor = vec4(ambient + diffuse, 1.0);
}
