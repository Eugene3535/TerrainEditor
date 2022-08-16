#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

class Texture2D
{
public:
    Texture2D() noexcept;
    Texture2D(const Texture2D& other) = delete;
	Texture2D(const Texture2D&& other) = delete;
	Texture2D& operator = (const Texture2D& other) = delete;
	Texture2D& operator = (const Texture2D&& other) = delete;
    ~Texture2D();

    bool loadFromFile(const std::string& filepath) noexcept;
    void bind(bool to_bind) noexcept;
    void setRepeated(bool repeat) noexcept;
    void setSmooth(bool smooth) noexcept;

    bool isRepeated() const noexcept;
    bool isSmooth() const noexcept;
    const glm::uvec2& getSize() const noexcept;

private:
    glm::ivec2 m_size;
    GLuint     m_handle;
    bool       m_is_repeated;
    bool       m_is_smooth;
};