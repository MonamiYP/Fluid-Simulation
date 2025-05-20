#pragma once

#include <iostream>

class Texture {
    private:
        unsigned int m_rendererID;
        std::string m_filepath;
        unsigned char* m_textureData;
        int m_width, m_height, m_BPP;

        void initTexture();

    public:
        Texture(const std::string& path); // Initialise texture with an image
        Texture(int width, int height); // Initialise texture 
        ~Texture();

        void UploadData(const std::vector<float>& data, const float color[3]);

        void Bind(unsigned int slot=0) const;
        void Unbind() const;

        inline int GetWidth() const { return m_width; }
        inline int GetHeight() const { return m_height; }
};