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

    // Сообщаем GLFW, чтобы он захватил наш курсор
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }    

    glEnable(GL_DEPTH_TEST);

    // Cell amount
    const int cells = 999;

    // Vertex amount as cells + 1
    const int vert_amount = 1000;

    std::vector<glm::vec3> vertices;
    vertices.resize(vert_amount * vert_amount, glm::vec3());

    float offsetW = 2.0f / cells;
    float offsetH = 2.0f / cells;

    int c = 0;

    for (int z = 0; z < vert_amount; z++)
    {
        for (int x = 0; x < vert_amount; x++)
        {
            vertices[c].x = -1.0f + x * offsetW;
            vertices[c].z = 1.0 + z * -offsetH;
            c++;
        }
    }

    c = 0;
    std::vector<GLuint> indices;
    indices.resize(cells * cells * 6);

    for (int y = 0; y < cells; y++)
    {
        for (int x = 0; x < cells; x++)
        {
            indices[c] = y * vert_amount + x;
            indices[c + 1] = y * vert_amount + x + 1;
            indices[c + 2] = y * vert_amount + x + vert_amount;

            c += 3;
        }
    }

    for (int y = 0; y < cells; y++)
    {
        for (int x = 1; x < vert_amount; x++)
        {
            indices[c] = y * vert_amount + x;
            indices[c + 1] = y * vert_amount + x + vert_amount;
            indices[c + 2] = y * vert_amount + x + vert_amount - 1;

            c += 3;
        }
    }

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
   
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
    model = glm::scale(model, glm::vec3(30, 30, 30));

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

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
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