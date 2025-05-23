#include "Texture.hpp"
#include "Renderer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &m_rendererID));
}

void Texture::initTexture() {
    GLCall(glGenTextures(1, &m_rendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}

Texture::Texture(int width, int height) : m_width(width), m_height(height) {
    initTexture();

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::Texture(const std::string& path) 
    : m_rendererID(0), m_filepath(path), m_textureData(nullptr), 
    m_width(0), m_height(0), m_BPP(0) {
        stbi_set_flip_vertically_on_load(1);

        initTexture();

        m_textureData = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 0);

         if (m_textureData) {
            GLenum format;
            if (m_BPP == 1) {format = GL_RED;}
            else if (m_BPP == 3) {format = GL_RGB;}
            else {format = GL_RGBA;}
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_textureData));
            GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }

        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        stbi_image_free(m_textureData);
}

void Texture::UploadData(const std::vector<float>& data, const float color[3]) {
    std::vector<float> rgbData(m_width * m_height * 3); // RGB, 3 floats per pixel

    for (int i = 0; i < m_width * m_height; i++) {
        float value = data[i];
        rgbData[i * 3 + 0] = value * color[0]; // Red
        rgbData[i * 3 + 1] = value * color[1]; // Green
        rgbData[i * 3 + 2] = value * color[2]; // Blue
    }

    glBindTexture(GL_TEXTURE_2D, m_rendererID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, rgbData.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
}

void Texture::Unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}