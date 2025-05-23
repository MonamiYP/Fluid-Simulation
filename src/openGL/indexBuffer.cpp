#include "IndexBuffer.hpp"
#include "Renderer.hpp"

IndexBuffer::IndexBuffer() {
    GLCall(glGenBuffers(1, &m_rendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &m_rendererID));
}

void IndexBuffer::AddData(const unsigned int* data, unsigned int count) {
    m_count = count;
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void IndexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void IndexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}