#include "Arrow.hpp"

Arrow::Arrow(int grid_width, int grid_height, Renderer& renderer) : m_grid_width(grid_width), m_grid_height(grid_height), m_start((grid_width * grid_height), 0.0f), m_end((grid_width * grid_height), 0.0f),
    m_vao(), m_vbo(), m_renderer(renderer) {

    VertexBufferLayout layout;
    layout.AddAttribute(2);
    m_vao.AddBuffer(m_vbo, layout);

    std::string vertex_source = m_shader.ParseShader("res/shaders/arrow.vertex.shader");
    std::string fragment_source = m_shader.ParseShader("res/shaders/arrow.fragment.shader");
    m_shader.CreateShaderProgram(vertex_source, fragment_source);

    glm::mat4 projection = glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f, -1.0f, 1.0f);
    m_shader.Bind();
    m_shader.SetMatrix4("u_projection", projection);
}

void Arrow::updateArrows(std::vector<float> x, std::vector<float> y) {
    float cellWidth = 1200.0f / (float)m_grid_width;
    float cellHeight = 800.0f / (float)m_grid_height;
    float arrowScale = 100.0f;

    std::vector<float> vertices(m_grid_width * m_grid_height * 4);
    int idx = 0;
    for (unsigned int i = 1; i <= m_grid_width; ++i) {
        for (unsigned int j = 1; j <= m_grid_height; ++j) {
            int index = i + (m_grid_width+2) * j;

            float startX = i * cellWidth;
            float startY = j * cellHeight;   
            
            float lengthX = x[index] * arrowScale;
            float lengthY = y[index] * arrowScale;

            float length = sqrt(lengthX * lengthX + lengthY * lengthY);
            float maxLength = 10.0f;

            if (length > maxLength) { 
                lengthX *= maxLength / length;
                lengthY *= maxLength / length;
            }

            float endX = startX + lengthX;
            float endY = startY + lengthY;

            vertices[idx++] = (startX);
            vertices[idx++] = (startY);
            vertices[idx++] = (endX);
            vertices[idx++] = (endY);
        }
    }

    m_vbo.Bind();
    m_vbo.AddData(vertices.data(), vertices.size() * sizeof(float));
}

void Arrow::drawArrows() {
    m_vao.Bind();
    m_shader.Bind();

    int vertexCount = 2 * m_grid_width * m_grid_height;
    glDrawArrays(GL_LINES, 0, vertexCount);

    m_vao.Unbind();
    m_shader.Unbind();
}