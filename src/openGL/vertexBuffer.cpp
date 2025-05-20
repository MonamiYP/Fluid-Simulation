#include "VertexBuffer.hpp"
#include "Renderer.hpp"

VertexBuffer::VertexBuffer() {
    GLCall(glGenBuffers(1, &m_rendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_rendererID));
}

void VertexBuffer::AddData(const void* data, unsigned int size) {
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

void VertexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void VertexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}