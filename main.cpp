#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

const int WIDTH = 600;
const int HEIGHT = 800;
const char* title = "Phantom Engine";
int main(){
    
    if(!glfwInit()) throw std::runtime_error("Failed to init GLFW");
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        //Processing Here and Render
        glfwSwapBuffers(window);
        glfwSwapInterval(0);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}