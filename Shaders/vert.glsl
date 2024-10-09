#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;

out vec3 aCol;

void main() {
    gl_Position = vec4(pos.xyz, 1);
    aCol = col;
}
