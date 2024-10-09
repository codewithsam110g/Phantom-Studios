#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;
layout(location = 2) in vec2 texCoords;

out vec3 aCol;
out vec2 aTexCoords;

void main() {
    gl_Position = vec4(pos.xyz, 1);
    aCol = col;
    aTexCoords = texCoords;
}
