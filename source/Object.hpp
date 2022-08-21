#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>

#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "Camera.hpp"

class TestObject
{
public:
    TestObject();
    ~TestObject();

    bool create(Texture2D* texture);
    void draw();

private:
    float  m_vertices[20]{};
    GLuint m_indices[6] = {
        0, 1, 2, 
        0, 2, 3  
    };

	Texture2D* m_texture = nullptr;    

	GLuint VAO = 0, VBO = 0, EBO = 0; 
};