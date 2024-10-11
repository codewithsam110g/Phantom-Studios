#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>

#include "Mouse.h"
#include "Keyboard.h"
#include "Window.h"
#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Camera.h"

// Frame timing
float lastFrame = 0, currentFrame = 0;

bool firstMouse = true;
float lastX = 400;
float lastY = 300;

float vertices[] = {
    // Position - 3         Color - 3               TexCoords - 2
     0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,       1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,       1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,       0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 1.0f,       0.0f, 1.0f,   // top left
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};

Camera camera(glm::vec3(0, 0, 3));

// Function Declarations
void ProcessInput(float delta);

int main(){

    if(!glfwInit()) throw std::runtime_error("Failed to init GLFW");
    Window window(WindowProps{800, 600, "Phantom Studios"});

    // GLFW Callbacks
    glfwSetCursorPosCallback(window.getWindow(), Mouse::mousePositionCB);
    glfwSetMouseButtonCallback(window.getWindow(), Mouse::mouseButtonCB);
    glfwSetKeyCallback(window.getWindow(), Keyboard::keyboardButtonCB);

    if(!gladLoadGL()) throw std::runtime_error("Failed to Load OpenGL Loader");

    VertexArray vao;

    Buffer vbo(GL_ARRAY_BUFFER, sizeof(vertices), vertices),
    ebo(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);

    vao.setAttribPointer(0, 3, sizeof(float) * 8, (void *)(0));
    vao.setAttribPointer(1, 3, sizeof(float) * 8, (void *)(sizeof(float) * 3));
    vao.setAttribPointer(2, 2, sizeof(float) * 8, (void *)(sizeof(float) * 6));

    vao.unbind();

    Shader shader("./Shaders/vert.glsl", "./Shaders/frag.glsl");

    Texture texture("./container.jpg");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); For Wireframe Rendering

    glm::mat4 model = glm::mat4(1);
    model = glm::rotate(model, glm::radians(-55.0f),glm::vec3(1, 0, 0));

    lastFrame = glfwGetTime();
    while (!glfwWindowShouldClose(window.getWindow())) {
        glfwPollEvents();

        // Input Processing and Rendering Here

        currentFrame = glfwGetTime();
        float delta = lastFrame - currentFrame;
        lastFrame = currentFrame;

        ProcessInput(delta);

        int focused = glfwGetWindowAttrib(window.getWindow(), GLFW_FOCUSED);
        if(!focused){
            firstMouse = true;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.4, 0.4, 0.4, 0.4);

        glViewport(0, 0, window.getWidth(), window.getHeight());

        model = glm::rotate(model, 10 * glm::sin(glm::radians(delta)), glm::vec3(0, 0, 1));
        glm::mat4 projection = glm::perspective(glm::radians(75.0f), window.getAspectRatio(), 0.01f, 100.0f);

        shader.bind();

        // Setting Uniforms

        shader.setMat4("model", model);
        shader.setMat4("view", camera.GetViewMatrix());
        shader.setMat4("projection", projection);

        vao.bind();
        texture.bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        Mouse::update();
        Keyboard::update();

        glfwSwapBuffers(window.getWindow());
        glfwSwapInterval(0);
    }

    glfwDestroyWindow(window.getWindow());
    glfwTerminate();
    return 0;
}

void ProcessInput(float dt){

    // Keyboard Movement
    if (Keyboard::isKeyDown(GLFW_KEY_Q))
        glfwTerminate();
    if (Keyboard::isKeyDown(GLFW_KEY_W))
        camera.ProcessKeyboard(FORWARD, dt);
    if (Keyboard::isKeyDown(GLFW_KEY_S))
        camera.ProcessKeyboard(BACKWARD, dt);
    if (Keyboard::isKeyDown(GLFW_KEY_A))
        camera.ProcessKeyboard(LEFT, dt);
    if (Keyboard::isKeyDown(GLFW_KEY_D))
        camera.ProcessKeyboard(RIGHT, dt);
    if (Keyboard::isKeyDown(GLFW_KEY_SPACE))
        camera.ProcessKeyboard(UP, dt);
    if (Keyboard::isKeyDown(GLFW_KEY_LEFT_CONTROL))
        camera.ProcessKeyboard(DOWN, dt);

    if(firstMouse){
        float x, y;
        Mouse::getCursorPosition(&x, &y);
        lastX = x;
        lastY = y;
        firstMouse = false;
    }else{
        // Mouse Movement
        float x, y;
        Mouse::getCursorPosition(&x, &y);
        float delX, delY;
        delX = x - lastX;
        delY = lastY - y;
        lastX = x;
        lastY = y;
        camera.ProcessMouseMovement(delX, delY);
    }

}
