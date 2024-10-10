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
    static void mousePosCB(GLFWwindow* window, double xpos, double ypos){
        LastX = X;
        LastY = Y;
        X = xpos;
        Y = ypos;
        delX = X - LastX;
        delY = Y - LastY;
    }
    static void getDelta(float *x, float *y){
        *x = delX;
        *y = delY;
    }
    static void mouseButtonCB(GLFWwindow* window, int button, int action, int mods){
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

    static bool isKeyDown(MouseButton btn){
        return keyMap[btn];
    }

    static bool isKeyPressed(MouseButton btn){
        return keyMap[btn] && !prevKeyMap[btn];
    }

    static bool isKeyReleased(MouseButton btn){
        return !keyMap[btn] && prevKeyMap[btn];
    }

    static void update(){
        for (int i = 0; i <= 2; i++) {
            prevKeyMap[i] = keyMap[i];
        }
    }
};


float Mouse::X = 0.0f;
float Mouse::Y = 0.0f;
float Mouse::LastX = 0.0f;
float Mouse::LastY = 0.0f;
float Mouse::delX = 0.0f;
float Mouse::delY = 0.0f;
bool Mouse::keyMap[3] = { false };
bool Mouse::prevKeyMap[3] = { false };

#endif
