#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "Camera.hpp"
#include "Terrain.hpp"
#include "TileMap.hpp"
#include "Object.hpp"
#include "Transform.hpp"

bool IsKeyPressed(GLFWwindow* window, const int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

Camera camera;
float lastX = SCR_WIDTH * 0.5f;
float lastY = SCR_HEIGHT * 0.5f;

bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    srand(time(NULL));

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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



    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Texture2D tGrass;
    bool d = tGrass.loadFromFile("resources/textures/field.png");
    tGrass.setRepeated(true);

    Texture2D tileset;
    tileset.loadFromFile("resources/textures/tileset.png");
 
    Terrain terrain;
    terrain.create(100, tGrass);

    TileMap tilemap;
    tilemap.create(100, tileset, "resources/maps/data.txt");

    ShaderProgram shader;
    shader.compileShader("resources/shaders/terrain.vert", GL_VERTEX_SHADER);
    shader.compileShader("resources/shaders/terrain.frag", GL_FRAGMENT_SHADER);

    shader.addUniform("model");
    shader.addUniform("view");
    shader.addUniform("projection");
    shader.use();

    // Matrices

    glm::mat4 terrain_model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    projection = glm::perspective(glm::radians(70.0f), 1200.f / 800.f, 0.001f, 100.0f);

    camera.init(view);

    glm::mat4 terrain_matrix = projection * view * terrain_model;
    shader.setUniform("projection", glm::value_ptr(terrain_matrix));

//  Objects on map
    Texture2D tTree;
    tTree.loadFromFile("resources/textures/flower0.png");

    TestObject oTree;
    oTree.create(&tTree);

    ShaderProgram oShader;
    oShader.compileShader("resources/shaders/object.vert", GL_VERTEX_SHADER);
    oShader.compileShader("resources/shaders/object.frag", GL_FRAGMENT_SHADER);
    oShader.use();

    

    Transform object_model;
    object_model.setPosition(glm::vec3(5, 0.5f, 5));

    oShader.addUniform("model");
    oShader.addUniform("view");
    oShader.addUniform("projection");
    oShader.use();

    oShader.setUniform("model", object_model.getMatrix());
    oShader.setUniform("projection", glm::value_ptr(terrain_matrix));

    int i = 0;
        
    while (!glfwWindowShouldClose(window))
    {      
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.update();

        shader.use();      
        shader.setUniform("view", glm::value_ptr(view));
        //terrain.draw();
        tilemap.draw();

        oShader.use();
    
        oShader.setUniform("view", glm::value_ptr(view));
        

        if(IsKeyPressed(window, GLFW_KEY_LEFT))
            object_model.move(glm::vec3(0.1f, 0, 0));

        if(IsKeyPressed(window, GLFW_KEY_RIGHT))
            object_model.move(glm::vec3(-0.1f, 0, 0));

        if(IsKeyPressed(window, GLFW_KEY_UP))
            object_model.move(glm::vec3(0, 0, 0.1f));

        if(IsKeyPressed(window, GLFW_KEY_DOWN))
            object_model.move(glm::vec3(0, 0, -0.1f));

        object_model.rotate(0.0f, glm::vec3(0, 1, 0));
        oShader.setUniform("model", object_model.getMatrix());
        oTree.draw();

        object_model.rotate(90.0f, glm::vec3(0, 1, 0));
        oShader.setUniform("model", object_model.getMatrix());
        oTree.draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();

        Sleep(16);
    }

    terrain.destroy();
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
    camera.setMouseViewOrientation(xpos, ypos);
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