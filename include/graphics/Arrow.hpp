#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

class Arrow {
    private:
        std::vector<float> m_start;
        std::vector<float> m_end;

        int m_grid_width;
        int m_grid_height;

        int m_size = m_grid_width * m_grid_height;

        VertexArray m_vao;
        VertexBuffer m_vbo;
        IndexBuffer m_ibo;
        Shader m_shader;
        Renderer m_renderer;

    public:
        Arrow(int grid_width, int grid_height, Renderer& renderer);

        void updateArrows(std::vector<float> x, std::vector<float> y);
        void drawArrows();
};