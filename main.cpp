#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

const int WIDTH = 600;
const int HEIGHT = 800;
const char* title = "Phantom Engine";

float vertices[] = {
    -0.5f, -0.5f, 0.0f, // left
     0.5f, -0.5f, 0.0f, // right
     0.0f,  0.5f, 0.0f  // top
};

int main(){

    if(!glfwInit()) throw std::runtime_error("Failed to init GLFW");
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if(!gladLoadGL()) throw std::runtime_error("Failed to Load OpenGL Loader");

    uint32_t vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)(0));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        //Processing Here and Render
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.4, 0.4, 0.4, 0.4);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwSwapInterval(0);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
