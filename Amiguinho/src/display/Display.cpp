#include "Display.h"
#include <iostream>
#include <vector>

Display::Display()
	: window_width(0), window_height(0),
	window(nullptr)
{
}

Display::~Display()
{
}

void Display::initialize_window()
{
	if (!glfwInit())
	{
		std::cout << "GLFW Failed to init!" << std::endl;
		glfwTerminate();

		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_FLOATING, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	window_width = mode->width;
	window_height = 720;

	window = glfwCreateWindow(window_width, window_height, "OpenGL 3D Renderer", NULL, NULL);
	
	glfwSetWindowPos(window, 0, mode->height - 600);

	if (!window)
	{
		std::cout << "GLFW failed to create window! \n";
		glfwTerminate();
		return;
	}

	glfwGetFramebufferSize(window, &buffer_width, &buffer_height);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	create_callbacks();

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init failed \n";
		glfwDestroyWindow(window);
		glfwTerminate();

		return;
	}

	//glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, buffer_width, buffer_height);
	glfwSetWindowUserPointer(window, this);
}

void Display::swap_buffers()
{
	glfwSwapBuffers(window);
}

void Display::clear_color(int r, int g, int b, int a)
{
	glClearColor(r, g, b, a);
}

void Display::clear()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
}


bool Display::should_close()
{
	return glfwWindowShouldClose(window);
}

void Display::create_callbacks()
{
	glfwSetKeyCallback(window, Display::handle_keys);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Display::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Display* display = static_cast<Display*>(glfwGetWindowUserPointer(window));
	glViewport(0, 0, width, height);

	display->window_width = width;
	display->window_height = height;
}

void Display::handle_keys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Display* display = static_cast<Display*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void Display::setup_render_mode(int key)
{
	switch (key)
	{
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 2:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	}
}