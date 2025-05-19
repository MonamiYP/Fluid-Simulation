#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

class DrawQuad {
    private:
        VertexArray m_vao;
        VertexBuffer m_vbo;
        IndexBuffer m_ibo;
        Shader m_shader;
        Renderer m_renderer;
        
    public:
        DrawQuad(Renderer& renderer);
        ~DrawQuad() {};

        void draw();
        void draw(Texture& tex);
};