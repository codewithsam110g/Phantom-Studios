#include "../include/Keyboard.h"

void Keyboard::keyboardButtonCB(GLFWwindow* window, int key, int scancode, int action, int mods){
    prevKeyMap[key] = keyMap[key];
    if(action == GLFW_PRESS){
        keyMap[key] = true;
    }else if(action == GLFW_RELEASE){
        keyMap[key] = false;
    }
}
void Keyboard::update(){
    std::memcpy(prevKeyMap, keyMap, sizeof(keyMap));
}

bool Keyboard::isKeyDown(int key){
    return keyMap[key];
}

bool Keyboard::isKeyPressed(int key){
    return keyMap[key] && !prevKeyMap[key];
}
bool Keyboard::isKeyRelease(int key){
    return !keyMap[key] && prevKeyMap[key];
}


bool Keyboard::keyMap[349] = {false};
bool Keyboard::prevKeyMap[349] = {false};
