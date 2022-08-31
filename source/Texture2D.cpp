#include "Texture2D.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_ONLY_TGA

#include "stb_image.h"

#include <iostream>
#include <map>

Texture2D::Texture2D()  noexcept: 
    m_size(),
    m_handle(0),
    m_is_repeated(false),
    m_is_smooth(false)
{
}

Texture2D::~Texture2D()
{
    if(m_handle) 
        glDeleteTextures(1, &m_handle);
}

bool Texture2D::loadFromFile(const std::string& filepath) noexcept
{
    if (m_handle) 
        glDeleteTextures(1, &m_handle);

    glGenTextures(1, &m_handle);

    setRepeated(false);
    setSmooth(false);

    stbi_set_flip_vertically_on_load(true);

    int channels = 0;
    unsigned char* data = stbi_load(filepath.c_str(), &m_size.x, &m_size.y, &channels, STBI_rgb_alpha);

    if (data)
    {
        bind(true);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        bind(false);

        stbi_image_free(data);      
        return true;
    }
    std::cerr << "Failed to load texture " + filepath + '\n';

    return false;
}

void Texture2D::bind(bool to_bind) noexcept
{
    to_bind ? 
        glBindTexture(GL_TEXTURE_2D, m_handle) : // on
        glBindTexture(GL_TEXTURE_2D, 0);         // off
}

void Texture2D::setRepeated(bool repeat) noexcept
{
    bind(true);

    if (repeat)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        m_is_repeated = true;
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        float border_color[] { 0.0f, 0.0f, 0.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

        m_is_repeated = false;
    }
    bind(false);
}

void Texture2D::setSmooth(bool smooth) noexcept
{
    bind(true);

    if (smooth)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        m_is_smooth = true;
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        m_is_smooth = false;
    }
    bind(false);
}

bool Texture2D::isRepeated() const noexcept
{
    return m_is_repeated;
}

bool Texture2D::isSmooth() const noexcept
{
    return m_is_smooth;
}

const glm::uvec2& Texture2D::getSize() const noexcept
{
    return m_size;
}