#include <iostream>

#include <glad/glad.h>

#include "window.h"
#include "Logger.h"

Window* Window::instance;

int main()
{

	Logger::EnableFileOutput("log.txt");

    Window* window = Window::getInstance(800,600, "Phantom Studios");

	gladLoadGL();
	while (!window->doesWindowClose())
	{
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		glfwSwapBuffers(window->getWindowHandle());

	}

	Logger::CloseFileOutput();

	return 0;
}