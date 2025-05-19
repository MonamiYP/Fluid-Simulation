#version 330 core

layout(location = 0) in vec4 position;

out vec2 v_TextureCoords;

void main() {
    gl_Position = position;
    v_TextureCoords = position.xy * 0.5 + 0.5; // Convert from [-1,1] clip coords to [0,1] texture coords
}