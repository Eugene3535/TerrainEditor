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
	void draw(ShaderProgram& shader);
	void destroy();

private:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_tex_coords;
	std::vector<GLuint>    m_indices;

	Texture2D* m_surface;    // Temporary solution, only for the surface cover

	GLuint VAO, VBO[2], EBO; // Handles: vertex array object, vertex buffers( vertices & texture coords ) and index buffer
};
