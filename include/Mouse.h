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
    // Cursor Position
    static float X, Y;
    static float LastX, LastY;
    static float delX, delY;

    // Mouse Button States
    static bool keyMap[3];
    static bool prevKeyMap[3];

    // Mouse Scroll
    static float scrollX, scrollY;
public:
    static void mousePositionCB(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCB(GLFWwindow* window, int button, int action, int mods);
    static void mouseScrollCB(GLFWwindow* window, double xOffset, double yOffset);

    static void getCursorPosition(float *x, float* y);
    static void getCursorDelta(float *x, float *y);

    static bool isKeyDown(MouseButton btn);
    static bool isKeyPressed(MouseButton btn);
    static bool isKeyReleased(MouseButton btn);

    static float getScrollDelta();

    static void update();
};


#endif
