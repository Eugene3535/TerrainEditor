#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "Camera.hpp"

class Terrain
{
public:
	Terrain();
	Terrain(const Terrain& other) = delete;
	Terrain(const Terrain&& other) = delete;
	Terrain& operator = (const Terrain& other) = delete;
	Terrain& operator = (const Terrain&& other) = delete;
	~Terrain();

	void create(std::size_t amount /* of vertices on same side */, Texture2D& texture /* surface cover */);
	void draw();
	void destroy();

	GLuint getHeightInPoint(GLuint point);

private:
	bool isCoordsInMap(GLuint position);
	void createHill(std::size_t x, std::size_t z, std::size_t radius, std::size_t height);

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_tex_coords;
	std::vector<GLuint>    m_indices;

	Texture2D* m_surface;    // Temporary solution, only for the surface cover
	glm::uvec2 m_map_size;

	GLuint VAO, VBO[2], EBO; // Handles: vertex array object, vertex buffers( vertices & texture coords ) and index buffer
	GLuint m_count = 0;
};
