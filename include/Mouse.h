#ifndef MOUSE_H
#define MOUSE_H

#include <GLFW/glfw3.h>

enum MouseButton{
    MOUSE_LEFT = 0,
    MOUSE_RIGHT = 1,
    MOUSE_MIDDLE = 2
};

class Mouse{
private:
    static float X, Y;
    static float LastX, LastY;
    static float delX, delY;
    static bool keyMap[3];
    static bool prevKeyMap[3];
public:
    static void mousePositionCB(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCB(GLFWwindow* window, int button, int action, int mods);

    static void getCursorPosition(float *x, float* y);
    static void getCursorDelta(float *x, float *y);
    
    static bool isKeyDown(MouseButton btn);
    static bool isKeyPressed(MouseButton btn);
    static bool isKeyReleased(MouseButton btn);

    static void update();
};


#endif
