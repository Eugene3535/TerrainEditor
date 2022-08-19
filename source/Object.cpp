#include "Object.hpp"

TestObject::TestObject()
{

}

TestObject::~TestObject()
{
    if(VAO)
        glDeleteVertexArrays(1, &VAO);

    if(VBO)
        glDeleteBuffers(1, &VBO);

    if(EBO)
        glDeleteBuffers(1, &EBO);
}

bool TestObject::create(Texture2D* texture)
{
    m_texture = texture;

//  X                      Y                      Z
    m_vertices[0]  = 0.0f; m_vertices[1]  = 0.0f; m_vertices[2]  = 0.0f;   m_vertices[3]  = 0.0f; m_vertices[4]  = 0.0f;
    m_vertices[5]  = 1.0f; m_vertices[6]  = 0.0f; m_vertices[7]  = 0.0f;   m_vertices[8]  = 1.0f; m_vertices[9]  = 0.0f;
    m_vertices[10] = 1.0f; m_vertices[11] = 1.0f; m_vertices[12] = 0.0f;   m_vertices[13] = 1.0f; m_vertices[14] = 1.0f;
    m_vertices[15] = 0.0f; m_vertices[16] = 1.0f; m_vertices[17] = 0.0f;   m_vertices[18] = 0.0f; m_vertices[19] = 1.0f;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, m_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, m_indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    return true;
}

void TestObject::render(ShaderProgram& shader)
{
    shader.use();

    glBindVertexArray(VAO);
    m_texture->bind(true);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    m_texture->bind(false);
    glBindVertexArray(0);
}