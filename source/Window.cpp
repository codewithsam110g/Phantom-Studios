#include "../include/Window.h"
#include <GLFW/glfw3.h>

Window::Window(WindowProps wp){
    windowProps = wp;
    glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
    window = glfwCreateWindow(windowProps.width, windowProps.height,
        windowProps.title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, resizeWindowCB);
}

GLFWwindow* Window::getWindow(){
    return window;
}
float Window::getAspectRatio(){
    return (float)windowProps.width / (float)windowProps.height;
}

int Window::getWidth(){
    return windowProps.width;
}
int Window::getHeight(){
    return windowProps.height;
}

bool Window::shouldWindowClose(){
    return glfwWindowShouldClose(window);
}

void Window::resizeWindowCB(GLFWwindow* window, int width, int height){
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->windowProps.width = width;
        win->windowProps.height = height;
    }
}

Window::~Window(){
    glfwDestroyWindow(window);
}
