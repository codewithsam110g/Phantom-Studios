#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Window.h>

WindowProps wp1{800, 600, "Window 1"};
WindowProps wp2{800, 600, "Window 2"};

int main(){

    glfwInit();
    Window w1(wp1), w2(wp2);

    gladLoadGL();
    while(!w1.shouldWindowClose() && !w2.shouldWindowClose()){
        glfwMakeContextCurrent(w1.getWindow());
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.4, 0.4, 0.4, 1);

        glfwSwapBuffers(w1.getWindow());
        glfwSwapInterval(0);

        glfwMakeContextCurrent(w2.getWindow());
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.7, 0.7, 0.7, 1);

        glfwSwapBuffers(w2.getWindow());
        glfwSwapInterval(0);
    }

    glfwTerminate();

}
