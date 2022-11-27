#define _USE_MATH_DEFINES
#include <cmath>

#include "TileMap.hpp"

#include <memory>
#include <fstream>
#include <sstream>

TileMap::TileMap() : m_surface(nullptr),
                     VAO(0),
                     VBO{0, 0},
                     EBO(0)
{
}

TileMap::~TileMap()
{
    destroy();
}

bool TileMap::create(std::size_t vertex_amount, Texture2D &texture, const std::string &filename)
{
    std::ifstream ifs_stream;
    std::vector<std::uint32_t> parsed_layer;

    ifs_stream.open(filename);

    if ( ! ifs_stream.is_open())
        return false;

    std::stringstream s_stream;
    s_stream << ifs_stream.rdbuf();
    ifs_stream.close();

    std::uint32_t tile_id = 0;

    while (s_stream >> tile_id)
        parsed_layer.push_back(tile_id);

    int tile_width = 192;
    int tile_height = 192;

    std::vector<glm::vec2> texture_grid; // Contains left - top coords of each tile in the tileset

	for (unsigned y = 0u; y < texture.getSize().y / 192; ++y)
		for (unsigned x = 0u; x < texture.getSize().x / 192; ++x)
			texture_grid.emplace_back(glm::vec2(x * tile_width, y * tile_height));

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> tex_coords;
    std::vector<GLuint> indices;

    m_map_size = {vertex_amount, vertex_amount};
    m_surface = std::addressof(texture);

    vertices.reserve((vertex_amount - 1) * (vertex_amount - 1) * 4);
    tex_coords.reserve(vertex_amount * vertex_amount * 4);
    indices.reserve((vertex_amount - 1) * (vertex_amount - 1) * 6);

    GLuint tile_num = 0;

    auto tex_size = texture.getSize();

    for (GLuint z = 0u; z < 99; ++z)
    {
        for (GLuint x = 0u; x < 99; ++x)
        {
            GLuint index = x + z * 99;
            GLuint tile_id = parsed_layer[index];

            if (tile_id) // Zero value means that current tile is empty
            {
                const glm::vec2 &texCoords = texture_grid[tile_id - 1];

                float left   = texCoords.x / tex_size.x;
                float top    = texCoords.y / tex_size.y;
                float right  = (texCoords.x + tile_width) / tex_size.x;
                float bottom = (texCoords.y + tile_height) / tex_size.y;

                vertices.push_back({x,     0, z + 1});
                vertices.push_back({x + 1, 0, z + 1});
                vertices.push_back({x + 1, 0, z});
                vertices.push_back({x,     0, z});

                tex_coords.push_back({left, bottom});
                tex_coords.push_back({right, bottom});
                tex_coords.push_back({right, top});
                tex_coords.push_back({left, top});

                // 1-st triangle
                indices.push_back(tile_num);
                indices.push_back(tile_num + 1);
                indices.push_back(tile_num + 2);

                // 2-nd triangle
                indices.push_back(tile_num);
                indices.push_back(tile_num + 2);
                indices.push_back(tile_num + 3);

                tile_num += 4;
            }
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * tex_coords.size(), tex_coords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    m_count = indices.size();

    return true;
}

void TileMap::draw()
{
    glBindVertexArray(VAO);
    m_surface->bind(true);

    glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, 0);

    m_surface->bind(false);
    glBindVertexArray(0);
}

void TileMap::destroy()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(1, &EBO);
}
