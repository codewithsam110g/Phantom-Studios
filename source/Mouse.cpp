#include "../include/Mouse.h"

void Mouse::mousePositionCB(GLFWwindow* window, double xpos, double ypos){
    LastX = X;
    LastY = Y;
    X = xpos;
    Y = ypos;
    delX = X - LastX;
    delY = Y - LastY;
}

void Mouse::mouseButtonCB(GLFWwindow* window, int button, int action, int mods){
    if(button <= 2){
        prevKeyMap[button] = keyMap[button];
        if(action == GLFW_PRESS){
            keyMap[button] = true;
        }
        else {
            keyMap[button] = false;
        }
    }
}

void Mouse::getCursorPosition(float *x, float *y){
    *x = X;
    *y = Y;
}

void Mouse::getCursorDelta(float *x, float *y){
    *x = delX;
    *y = delY;
}

bool Mouse::isKeyDown(MouseButton btn){
    return keyMap[btn];
}

bool Mouse::isKeyPressed(MouseButton btn){
    return keyMap[btn] && !prevKeyMap[btn];
}

bool Mouse::isKeyReleased(MouseButton btn){
    return !keyMap[btn] && prevKeyMap[btn];
}

void Mouse::update(){
    for (int i = 0; i <= 2; i++) {
        prevKeyMap[i] = keyMap[i];
    }
}

float Mouse::X = 0.0f;
float Mouse::Y = 0.0f;
float Mouse::LastX = 0.0f;
float Mouse::LastY = 0.0f;
float Mouse::delX = 0.0f;
float Mouse::delY = 0.0f;
bool Mouse::keyMap[3] = { false };
bool Mouse::prevKeyMap[3] = { false };
