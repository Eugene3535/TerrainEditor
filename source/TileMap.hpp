#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "Camera.hpp"

class TileMap
{
public:
	TileMap();
	TileMap(const TileMap& other) = delete;
	TileMap(const TileMap&& other) = delete;
	TileMap& operator = (const TileMap& other) = delete;
	TileMap& operator = (const TileMap&& other) = delete;
	~TileMap();

	bool create(std::size_t vertex_amount, Texture2D& texture, const std::string& filename);
	void draw();
	void destroy();

	GLuint getHeightInPoint(GLuint point);

private:
	bool isCoordsInMap(GLuint position);

	Texture2D* m_surface;    
	glm::uvec2 m_map_size;

	GLuint VAO, VBO[2], EBO; // Handles: vertex array object, vertex buffers( vertices & texture coords ) and index buffer
    GLuint m_count = 0;
};
