#define _USE_MATH_DEFINES
#include <cmath>

#include "Terrain.hpp"

#include <memory>

Terrain::Terrain():
    m_surface(nullptr),
    VAO(0),
    VBO{0, 0},
    EBO(0)
{
}

Terrain::~Terrain()
{
    destroy();
}

void Terrain::create(std::size_t vertex_amount, Texture2D& texture)
{
    m_map_size = { vertex_amount , vertex_amount };
    m_surface = std::addressof(texture);

    m_vertices.resize(vertex_amount * vertex_amount, glm::vec3());
    m_tex_coords.resize(vertex_amount * vertex_amount, glm::vec2());
    m_indices.resize((vertex_amount - 1) * (vertex_amount - 1) * 6);

    for (size_t z = 0; z < vertex_amount; z++)
    {
        for (size_t x = 0; x < vertex_amount; x++)
        {
            std::size_t pos = z * vertex_amount + x;

            m_vertices[pos].x = x;
            //m_vertices[pos].y = (rand() % 10) * 0.05f;
            m_vertices[pos].z = z;

            m_tex_coords[pos].x = x;
            m_tex_coords[pos].y = z;
        }
    }

    for (size_t i = 0; i < 10; i++)
    {
        std::size_t x = rand() % m_map_size.x;
        std::size_t y = rand() % m_map_size.y;
        std::size_t radius = rand() % 50;
        std::size_t height = rand() % 10;
        
        //createHill(x, y, radius, height);
    }

    // createHill(30, 30, 20, 5);

    std::size_t index = 0;
    const std::size_t cell_amount = vertex_amount - 1;

    for (size_t y = 0; y < cell_amount; y++)
    {
        for (size_t x = 0; x < cell_amount; x++)
        {
            GLuint pos = y * vertex_amount + x;

            m_indices[index] = pos;
            m_indices[index + 1] = pos + 1;
            m_indices[index + 2] = pos + vertex_amount;

            m_indices[index + 3] = pos + 1;
            m_indices[index + 4] = pos + vertex_amount;
            m_indices[index + 5] = pos + vertex_amount + 1;

            index += 6;
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_tex_coords.size(), m_tex_coords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Terrain::draw()
{
    glBindVertexArray(VAO);
    m_surface->bind(true);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    m_surface->bind(false);
    glBindVertexArray(0);
}

void Terrain::destroy()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(1, &EBO);
}

bool Terrain::isCoordsInMap(std::size_t position)
{
    return (position > 0 && position < m_vertices.size());
}

void Terrain::createHill(std::size_t x, std::size_t z, std::size_t radius, std::size_t height)
{
    if (m_vertices.empty())
        return;

    for (size_t i = z - radius; i < z + radius; i++)
    {
        for (size_t j = x - radius; j < x + radius; j++)
        {
            if (std::size_t position = i * m_map_size.x + j; isCoordsInMap(position))
            {
                float distance = sqrtf((x - j) * (x - j) + (z - i) * (z - i));
                
                if (distance < radius)
                {
                    distance = distance / radius * M_PI_2;
                    m_vertices[position].y += cos(distance) * height;
                }  
            }          
        }
    }
}
