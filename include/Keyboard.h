#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GLFW/glfw3.h>
#include <cstring>
#include <initializer_list>

class Keyboard {
private:
    static bool keyMap[349];           
    static bool prevKeyMap[349];

    static bool ctrlPressed;       
    static bool shiftPressed;      
    static bool altPressed;        

public:
    static void keyboardButtonCB(GLFWwindow* window, int key, int scancode, int action, int mods);

    static bool isKeyDown(int key);

    static bool isKeyPressed(int key);

    static bool isKeyRelease(int key);

    static bool isCtrlPressed();

    static bool isShiftPressed();

    static bool isAltPressed();

    static bool isMultiComboPressed(std::initializer_list<int> keys);
    
    static bool isSingleMultiComboPressed(std::initializer_list<int> keys);
    
    static void update();
};

#endif
