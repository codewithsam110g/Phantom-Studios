#define WINDOW_H
#include "Logger.h"
#include <GLFW/glfw3.h>

class Window {
private:
	GLFWwindow* window;
	int width, height;
	const char* title;

	Window() {

		Logger::Info("Starting to Init Window Object!");

		width = 800;
		height = 600;
		title = "Default!";

		if (glfwInit() != GLFW_TRUE) {
			Logger::Critical("Glfw Cannot Initialize");
		}

		glfwDefaultWindowHints();
		window = glfwCreateWindow(width, height, title, NULL, NULL);

		if (window == NULL) {
			Logger::Error("Could not start window!");
		}

		glfwMakeContextCurrent(window);

		Logger::Info("Window Object initialized!");

	}

	Window(int new_width, int new_height, const char* new_title) {

		Logger::Info("Starting to Init Window Object!");

		width = new_width;
		height = new_height;
		title = new_title;

		if (glfwInit() != GLFW_TRUE) {
			Logger::Critical("Glfw Cannot Initialize");
		}

		glfwDefaultWindowHints();
		window = glfwCreateWindow(width, height, title, NULL, NULL);

		if (window == NULL) {
			Logger::Error("Could not start window!");
		}

		glfwMakeContextCurrent(window);

		Logger::Info("Window Object initialized!");

	}

private:
	static Window* instance;

public:
	static Window* getInstance() {

		if (!Window::instance) {

			Window::instance = new Window();

		}

		return instance;


	}

	static Window* getInstance(int new_width, int new_height, const char* new_title) {

		if (!Window::instance) {

			Window::instance = new Window(new_width, new_height, new_title);

		}

		return instance;


	}

	GLFWwindow* getWindowHandle() {
		return window;
	}

	bool doesWindowClose() {
		return glfwWindowShouldClose(window);
	}

	void setWindowExtents(int new_width, int new_height) {
		width = new_width;
		height = new_height;
		glfwSetWindowSize(window, width, height);
	}

	void setWindowTitle(const char* new_title) {
		title = new_title;
		glfwSetWindowTitle(window, title);
	}

};