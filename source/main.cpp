#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "Camera.hpp"

bool IsKeyPressed(GLFWwindow* window, const int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void processInput(GLFWwindow* window);

// Константы
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// Камера
Camera camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;

// Тайминги
float deltaTime = 0.0f;	// время между текущим кадром и последним кадром
float lastFrame = 0.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Terrain editor", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }    

    glEnable(GL_DEPTH_TEST);
 
    const std::size_t vertex_amount = 2;
    const std::size_t cell_amount = vertex_amount - 1;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> tex_coords;
    std::vector<GLuint> indices;

    vertices.resize(vertex_amount * vertex_amount, glm::vec3());
    tex_coords.resize(vertex_amount * vertex_amount, glm::vec2());
    indices.resize(cell_amount * cell_amount * 6);

    float offsetZ = 2.0f / cell_amount;
    float offsetX = 2.0f / cell_amount;

    std::size_t index = 0;

    float Z = -1;

    for (size_t z = 0; z < vertex_amount; z++)
    {
        float X = -1;

        for (size_t x = 0; x < vertex_amount; x++)
        {
            vertices[index].x = X;
            vertices[index].z = Z;

            tex_coords[index].x = (X + 1) * 0.5f;
            tex_coords[index].y = (Z + 1) * 0.5f;

            X += offsetX;

            index++;
        }
        Z += offsetZ;
    }

    index = 0;

    for (size_t y = 0; y < cell_amount; y++)
    {
        for (size_t x = 0; x < cell_amount; x++)
        {
            GLuint pos = y * vertex_amount + x;

            indices[index] = pos;
            indices[index + 1] = pos + 1;
            indices[index + 2] = pos + vertex_amount + 1;

            indices[index + 3] = pos + 1;
            indices[index + 4] = pos + vertex_amount;
            indices[index + 5] = pos + vertex_amount + 1;

            index += 6;
        }
    }

    GLuint VAO, VBO[2], EBO;

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

    ShaderProgram shader;
    bool a = shader.loadFromFile("resources/shaders/shader.vert", GL_VERTEX_SHADER);
    bool b = shader.loadFromFile("resources/shaders/shader.frag", GL_FRAGMENT_SHADER);

    Texture2D grass;
    bool d = grass.loadFromFile("resources/textures/map1.png");
    grass.setRepeated(true);

    shader.addUniform("model");
    shader.addUniform("projection");
    camera.init(shader);
    shader.use();

    glm::mat4 model(1.0f);
    //model = glm::scale(model, glm::vec3(30, 30, 30));
    //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1));

    glm::mat4 projection(1);
    projection = glm::perspective(glm::radians(70.0f), 1200.f / 800.f, 0.001f, 100.0f);

    shader.setUniform("model", glm::value_ptr(model));
    shader.setUniform("projection", glm::value_ptr(projection));
        
    while (!glfwWindowShouldClose(window))
    {      
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        camera.apply(shader);

        glBindVertexArray(VAO);
        grass.bind(true);       

        glDrawElements(GL_TRIANGLE_FAN, indices.size(), GL_UNSIGNED_INT, 0);

        grass.bind(false);
        glBindVertexArray(0);  

        
        glfwSwapBuffers(window);
        glfwPollEvents();

        Sleep(16);
    }


    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.strafeLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.strafeRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.moveUp(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.moveDown(deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.mouseViewOrient(xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_LEFT_ALT)
        {
            glfwSetInputMode(window, GLFW_CURSOR, (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            camera.setMouseLook((glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) ? false : true);
        }
    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        std::cout << "boobs";
}