#include "../include/Keyboard.h"

void Keyboard::keyboardButtonCB(GLFWwindow* window, int key, int scancode, int action, int mods) {
    prevKeyMap[key] = keyMap[key];

    if (action == GLFW_PRESS) {
        keyMap[key] = true;
    } else if (action == GLFW_RELEASE) {
        keyMap[key] = false;
    }

    // Track modifier keys as well (Ctrl, Shift, Alt)
    ctrlPressed = mods & GLFW_MOD_CONTROL;
    shiftPressed = mods & GLFW_MOD_SHIFT;
    altPressed = mods & GLFW_MOD_ALT;
}

void Keyboard::update() {
    std::memcpy(prevKeyMap, keyMap, sizeof(keyMap));
}

bool Keyboard::isKeyDown(int key) {
    return keyMap[key];
}

bool Keyboard::isKeyPressed(int key) {
    return keyMap[key] && !prevKeyMap[key];
}

bool Keyboard::isKeyRelease(int key) {
    return !keyMap[key] && prevKeyMap[key];
}

bool Keyboard::isCtrlPressed() {
    return ctrlPressed;
}

bool Keyboard::isShiftPressed() {
    return shiftPressed;
}

bool Keyboard::isAltPressed() {
    return altPressed;
}

bool Keyboard::isMultiComboPressed(std::initializer_list<int> keys) {
    for (int key : keys) {
        if (!keyMap[key]) {
            return false;
        }
    }
    return true;
}

bool Keyboard::isSingleMultiComboPressed(std::initializer_list<int> keys) {
    bool nonModKeyPressed = false;

    for (int key : keys) {
        if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
            if (!ctrlPressed) return false; // Ensure Ctrl is held
        } else if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
            if (!shiftPressed) return false; // Ensure Shift is held
        } else if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) {
            if (!altPressed) return false; // Ensure Alt is held
        } else {
            // For non-modifier keys, ensure it is newly pressed
            if (keyMap[key] && !prevKeyMap[key]) {
                nonModKeyPressed = true;
            } else {
                return false; // Non-modifier key condition not met
            }
        }
    }

    return nonModKeyPressed; // Only true if at least one non-modifier key is newly pressed
}


// Initialize static variables
bool Keyboard::keyMap[349] = { false };
bool Keyboard::prevKeyMap[349] = { false };
bool Keyboard::ctrlPressed = false;
bool Keyboard::shiftPressed = false;
bool Keyboard::altPressed = false;
