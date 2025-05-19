#version 330 core

in vec2 v_TextureCoords;

out vec4 FragColor;

uniform sampler2D u_density;

void main() {
    float density = texture(u_density, v_TextureCoords).r;
    FragColor = vec4(density, density, density, 1.0);
}