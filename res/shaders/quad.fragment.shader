#version 330 core

in vec2 v_TextureCoords;
out vec4 FragColor;

uniform sampler2D u_density;

void main() {
    vec3 density = texture(u_density, v_TextureCoords).rgb;
    FragColor = vec4(density, 1.0);
}