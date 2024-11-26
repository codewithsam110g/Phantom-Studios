#version 330 core

in vec3 aCol;
in vec2 aTexCoords;

out vec4 Color;
uniform sampler2D t1;
void main() {
    //Color = vec4(aCol.xyz, 1);
    //Color = texture(t1, aTexCoords);
    //Color = texture(t1, aTexCoords) + vec4(aCol.xyz, 1);
    Color = mix(texture(t1, aTexCoords), vec4(aCol.xyz, 1), 0.2);
}
