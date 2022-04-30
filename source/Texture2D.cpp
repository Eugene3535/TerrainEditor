#include "Texture2D.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_ONLY_TGA

#include "stb_image.h"

#include <iostream>
#include <map>

Texture2D::Texture2D() : 
    m_handle(0), 
    m_size()
{
}

Texture2D::~Texture2D()
{
    if(m_handle) 
        glDeleteTextures(1, &m_handle);
}

bool Texture2D::loadFromFile(const std::string& filepath)
{
    if (m_handle) 
        glDeleteTextures(1, &m_handle);

    glGenTextures(1, &m_handle);

    setRepeated(false);
    setSmooth(false);

    stbi_set_flip_vertically_on_load(true);
 
    int width, height, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

    m_size = { width, height };

    GLenum mode = channels == 4 ? GL_RGBA : GL_RGB;

    if (data)
    {
        bind(true);
        glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        bind(false);

        stbi_image_free(data);      
        return true;
    }
    std::cerr << "Failed to load texture " + filepath + '\n';

    return false;
}

void Texture2D::bind(bool to_bind)
{
    to_bind ? 
        glBindTexture(GL_TEXTURE_2D, m_handle) : // on
        glBindTexture(GL_TEXTURE_2D, 0);         // off
}

void Texture2D::setRepeated(bool repeat)
{
    bind(true);

    if (repeat)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        float border_color[] { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
    }
    bind(false);
}

void Texture2D::setSmooth(bool smooth)
{
    bind(true);

    if (smooth)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    bind(false);
}

const glm::uvec2& Texture2D::getSize() const
{
    return m_size;
}