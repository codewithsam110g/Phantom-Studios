#version 330 core

in vec3 aCol;
in vec2 aTexCoords;

out vec4 Color;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main() {
    Color = mix(texture(texture_diffuse1, aTexCoords), texture(texture_specular1, aTexCoords), 0.5);
}
