#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GLFW/glfw3.h>
#include <cstring>

class Keyboard{
private:
    static bool keyMap[349];
    static bool prevKeyMap[349];

public:
    static void keyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods){
        prevKeyMap[key] = keyMap[key];
        if(action == GLFW_PRESS){
            keyMap[key] = true;
        }else if(action == GLFW_RELEASE){
            keyMap[key] = false;
        }
    }
    static void update(){
        std::memcpy(prevKeyMap, keyMap, sizeof(keyMap));
    }

    static bool isKeyDown(int key){
        return keyMap[key];
    }

    static bool isKeyPressed(int key){
        return keyMap[key] && !prevKeyMap[key];
    }
    static bool isKeyRelease(int key){
        return !keyMap[key] && prevKeyMap[key];
    }

};

bool Keyboard::keyMap[349] = {false};
bool Keyboard::prevKeyMap[349] = {false};

#endif
