#pragma once
#ifndef CLASS_WINDOW_H
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	~Window();

private:
	GLFWwindow* window;
	int width, height;
	const char* tittle;
};

Window::Window(){

	width = 800;
	height = 600;
	tittle = "Default!";
}

Window::~Window(){

}
#endif // !CLASS_WINDOW_H