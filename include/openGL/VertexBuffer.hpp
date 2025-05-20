#pragma once

class VertexBuffer {
    private:
        unsigned int m_rendererID;
    public:
        VertexBuffer();
        ~VertexBuffer();

        void AddData(const void* data, unsigned int size);

        void Bind() const;
        void Unbind() const;
};