#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

class Texture2D
{
public:
    Texture2D();
    ~Texture2D();

    bool loadFromFile(const std::string& filepath);
    void bind(bool to_bind);
    void setRepeated(bool repeat);
    void setSmooth(bool smooth);
    const glm::uvec2& getSize() const;

private:
    GLuint     m_handle;
    glm::uvec2 m_size;
};