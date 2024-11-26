#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <string>

struct WindowProps{
    int width;
    int height;
    std::string title;
};

class Window{
private:
    GLFWwindow* window;
    WindowProps windowProps;

    Window();

public:
    Window(WindowProps wp);

    GLFWwindow* getWindow();
    float getAspectRatio();

    int getWidth();
    int getHeight();
    void setWidthandHeight(int width, int height);

    bool shouldWindowClose();
    void setWindowClose(bool value);

    static void resizeWindowCB(GLFWwindow* window, int width, int height);

    ~Window();

};

#endif
