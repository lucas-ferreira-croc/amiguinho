#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

unsigned int vao;
unsigned int vbo;
unsigned int ibo;
unsigned int shader;

static const char* vShader = "                                                \n\
#version 330                                                                  \n\
out vec2 TexCoord;                                                       \n\
layout (location = 0) in vec3 pos;											  \n\
layout (location = 1) in vec2 tex;											  \n\
uniform mat4 model;                                                           \n\
void main()                                                                   \n\
{                                                                             \n\
    gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0);		  \n\
    TexCoord = tex;		  \n\
}";

// Fragment Shader
static const char* fShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
out vec4 colour;                                                               \n\
in vec2 TexCoord;                                                                          \n\
uniform sampler2D theTexture;                                                                          \n\
void main()                                                                   \n\
{                                                                             \n\
    colour = texture(theTexture, TexCoord);                                         \n\
}";


void create_triangle()
{

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 1
	};

	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  1.0f, 0.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		printf("Failed to create shader\n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

}


int main(int argc, char* argv[])
{
	
	if (!glfwInit())
	{
		std::cout << "GLFW Failed to init!" << std::endl;
		glfwTerminate();

		return -1;
	}
	
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//
	glfwWindowHint(GLFW_FLOATING, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
	
	
	unsigned int window_width = 3280, window_height = 720;
	

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	
	window_width = mode->width;
	//window_height = mode->height - 1;

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "OpenGL 3D Renderer", NULL, NULL);

	glfwSetWindowPos(window, 0, mode->height - 600);


	if (!window)
	{
		std::cout << "GLFW failed to create window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	int buffer_width, buffer_height;

	glfwGetFramebufferSize(window, &buffer_width, &buffer_height);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//create_callbacks();

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init failed" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();

		return -1;
	}

	//glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, buffer_width, buffer_height);
	//glfwSetWindowUserPointer(window, this);
	create_triangle();
	CompileShaders();


	/// <summary>
	unsigned int textureID;

	int width, height, bitDepth;
	std::string fileLocation = "C:\\dev\\amiguinho\\Amiguinho\\Amiguinho\\assets\\textures\\agumoles.png";
	stbi_set_flip_vertically_on_load(true);
	unsigned char* texData = stbi_load(fileLocation.c_str(), &width, &height, &bitDepth, 0);
	if (!texData)
	{
		std::cout << "Failed to find:" << fileLocation << "\n";
		return -1;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);

	/// </summary>
	/// <param name="argc"></param>
	/// <param name="argv"></param>
	/// <returns></returns>

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glUseProgram(shader);
			glBindVertexArray(vao);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
					glm::mat4 model(1.0f);
					model = glm::scale(model, glm::vec3(0.4f, 0.4, 0.4f));
					model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
					glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, textureID);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		glUseProgram(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate(); 

	return 0;
}