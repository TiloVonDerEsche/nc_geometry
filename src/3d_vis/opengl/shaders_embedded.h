// ==========================================
// AUTO-GENERATED FILE. DO NOT EDIT DIRECTLY.
// Generated from vertex.glsl and fragment.glsl
// ==========================================

#ifndef SHADERS_EMBEDDED_H
#define SHADERS_EMBEDDED_H

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec3 aStart;\n"
    "layout (location = 3) in vec3 aEnd;\n"
    "layout (location = 4) in vec2 aRadii;\n"
    "layout (location = 5) in vec3 aColor;\n"
    "\n"
    "out vec3 FragPos;\n"
    "out vec3 Normal;\n"
    "out vec3 TrackColor;\n"
    "\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec3 dz = aEnd - aStart;\n"
    "    float lengthVal = length(dz);\n"
    "    if (lengthVal == 0.0) lengthVal = 0.0001;\n"
    "    vec3 dir = dz / lengthVal;\n"
    "\n"
    "    vec3 up = vec3(0.0, 0.0, 1.0);\n"
    "    float dotUp = up.z * dir.z;\n"
    "    vec3 v = up - dotUp * dir;\n"
    "    float vLen = length(v);\n"
    "    if (vLen > 0.0) {\n"
    "        v /= vLen;\n"
    "    } else {\n"
    "        v = vec3(1.0, 0.0, 0.0);\n"
    "    }\n"
    "    vec3 u = cross(dir, v);\n"
    "\n"
    "    mat3 rotMat = mat3(u, v, dir);\n"
    "\n"
    "    vec3 scaledPos = vec3(aPos.x * aRadii.x, aPos.y * aRadii.y, aPos.z * lengthVal);\n"
    "    vec3 worldPos = aStart + rotMat * scaledPos;\n"
    "\n"
    "    FragPos = worldPos;\n"
    "    Normal = normalize(rotMat * aNormal);\n"
    "    TrackColor = aColor;\n"
    "\n"
    "    gl_Position = projection * view * vec4(worldPos, 1.0);\n"
    "}\n";

const char* fragmentShaderSource =
    "#version 330 core\n"
    "in vec3 FragPos;\n"
    "in vec3 Normal;\n"
    "in vec3 TrackColor;\n"
    "\n"
    "out vec4 FragColor;\n"
    "\n"
    "uniform float ambientIntensity;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec3 norm = normalize(Normal);\n"
    "    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));\n"
    "\n"
    "    float diff = max(dot(norm, lightDir), 0.0);\n"
    "    vec3 ambient = ambientIntensity * TrackColor;\n"
    "    vec3 diffuse = diff * TrackColor;\n"
    "\n"
    "    FragColor = vec4(ambient + diffuse, 1.0);\n"
    "}\n";


#endif // SHADERS_EMBEDDED_H
