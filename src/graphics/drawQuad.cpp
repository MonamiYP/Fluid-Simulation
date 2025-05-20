#include "DrawQuad.hpp"

DrawQuad::DrawQuad(Renderer& renderer) : m_vao(), m_vbo(), m_ibo(), m_renderer(renderer) {
    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
    m_vbo.AddData(vertices, sizeof(vertices));
    m_ibo.AddData(indices, 6);

    VertexBufferLayout layout;
    layout.AddAttribute(3);
    m_vao.AddBuffer(m_vbo, layout);

    std::string vertex_source = m_shader.ParseShader("res/shaders/quad.vertex.shader");
    std::string fragment_source = m_shader.ParseShader("res/shaders/quad.fragment.shader");
    m_shader.CreateShaderProgram(vertex_source, fragment_source);
}

void DrawQuad::draw() {
    m_shader.Bind();
    m_renderer.Draw(m_vao, m_ibo, m_shader);
}

void DrawQuad::draw(Texture& tex) {
    m_shader.Bind();
    tex.Bind(0);
    m_shader.SetInt("u_density", 0);
    m_renderer.Draw(m_vao, m_ibo, m_shader);
}