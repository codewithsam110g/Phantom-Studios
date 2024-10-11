#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GLFW/glfw3.h>
#include <cstring>

class Keyboard{
private:
    static bool keyMap[349];
    static bool prevKeyMap[349];

public:
    static void keyboardButtonCB(GLFWwindow* window, int key, int scancode, int action, int mods);

    static bool isKeyDown(int key);
    static bool isKeyPressed(int key);
    static bool isKeyRelease(int key);

    static void update();
};

#endif
