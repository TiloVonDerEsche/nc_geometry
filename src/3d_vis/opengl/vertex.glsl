#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aStart;
layout (location = 3) in vec3 aEnd;
layout (location = 4) in vec2 aRadii;
layout (location = 5) in vec3 aColor;

out vec3 FragPos;
out vec3 Normal;
out vec3 TrackColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec3 dz = aEnd - aStart;
    float lengthVal = length(dz);
    if (lengthVal == 0.0) lengthVal = 0.0001;
    vec3 dir = dz / lengthVal;

    vec3 up = vec3(0.0, 0.0, 1.0);
    float dotUp = up.z * dir.z;
    vec3 v = up - dotUp * dir;
    float vLen = length(v);
    if (vLen > 0.0) {
        v /= vLen;
    } else {
        v = vec3(1.0, 0.0, 0.0);
    }
    vec3 u = cross(dir, v);

    mat3 rotMat = mat3(u, v, dir);

    vec3 scaledPos = vec3(aPos.x * aRadii.x, aPos.y * aRadii.y, aPos.z * lengthVal);
    vec3 worldPos = aStart + rotMat * scaledPos;

    FragPos = worldPos;
    Normal = normalize(rotMat * aNormal);
    TrackColor = aColor;

    gl_Position = projection * view * vec4(worldPos, 1.0);
}
