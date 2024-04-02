#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <memory>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "display/Display.h"
#include "shader/Shader.h"
#include "texture/Texture.h"
#include "meshes/Mesh.h"

unsigned int vao;
unsigned int vbo;
unsigned int ibo;

std::vector<std::unique_ptr<Shader>> shaders;
std::vector<std::unique_ptr<Mesh>> meshes;

std::shared_ptr<Texture> texture;

void create_mesh()
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

	meshes.push_back(std::make_unique<Mesh>());
	meshes[0]->create_mesh(vertices, indices, 20, 6);
}

void create_shaders()
{
	shaders.push_back(std::make_unique<Shader>());
	shaders[0]->create_from_file(
		"C:\\dev\\amiguinho\\Amiguinho\\Amiguinho\\assets\\shaders\\vertex.glsl",
		"C:\\dev\\amiguinho\\Amiguinho\\Amiguinho\\assets\\shaders\\fragment.glsl"
	);
}

void setup(const Display& display)
{
	create_mesh();
	create_shaders();

	texture = std::make_shared<Texture>("C:\\dev\\amiguinho\\Amiguinho\\Amiguinho\\assets\\textures\\tom.png");
	texture->load_textureA();
}

void render(Display& display)
{
	display.clear();
	display.clear_color(0.0f, 0.0f, 0.0f, 0.0f);

	shaders[0]->bind();
		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(0.4f, 0.4, 0.4f));
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		shaders[0]->set_mat4("model", model);
		texture->use();
		meshes[0]->render();
	shaders[0]->unbind();

	display.swap_buffers();

}


int main(int argc, char* argv[])
{
	Display display;
	display.initialize_window();

	setup(display);

	while (!display.should_close())
	{
		render(display);
		glfwPollEvents();
	}

	glfwTerminate(); 

	return 0;
}