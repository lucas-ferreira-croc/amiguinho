#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <memory>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <Windows.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"

#include "display/Display.h"
#include "shader/Shader.h"
#include "texture/Texture.h"
#include "meshes/Mesh.h"
#include "Amiguinho/Amiguinho.h"

unsigned int vao;
unsigned int vbo;
unsigned int ibo;

std::vector<std::unique_ptr<Shader>> shaders;
std::vector<std::unique_ptr<Mesh>> meshes;


std::unique_ptr<Amiguinho> amiguinho;


void create_shaders()
{
	shaders.push_back(std::make_unique<Shader>());
	shaders[0]->create_from_file(
		"C:\\dev\\amiguinho\\Amiguinho\\Amiguinho\\assets\\shaders\\vertex.glsl",
		"C:\\dev\\amiguinho\\Amiguinho\\Amiguinho\\assets\\shaders\\fragment.glsl"
	);
}

void setup()
{
	amiguinho = std::make_unique<Amiguinho>();

	create_shaders();

	std::shared_ptr<Texture> texture = std::make_shared<Texture>("C:\\dev\\amiguinho\\Amiguinho\\Amiguinho\\assets\\textures\\tom.png");
	texture->load_textureA();

	std::shared_ptr<Texture> texture_falling = std::make_shared<Texture>("C:\\dev\\amiguinho\\Amiguinho\\Amiguinho\\assets\\textures\\tom_falling.png");;
	texture_falling->load_textureA();

	std::shared_ptr<Texture> texture_grabbed = std::make_shared<Texture>("C:\\dev\\amiguinho\\Amiguinho\\Amiguinho\\assets\\textures\\being_grabbed.jpg");
	texture_grabbed->load_texture();

	amiguinho->emplace_texture_map("default", texture);
	amiguinho->emplace_texture_map("falling", texture_falling);
	amiguinho->emplace_texture_map("grabbed", texture_grabbed);
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(amiguinho->display.get_window(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

}

void render()
{
	amiguinho->display.clear();
	amiguinho->display.clear_color(0.0f, 0.0f, 0.0f, 0.0f);

	///
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	///
	
	shaders[0]->bind();
		glm::mat4 model(1.0f);
		//model = glm::scale(model, glm::vec3(0.4f, 0.4, 0.4f));
		//model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		shaders[0]->set_mat4("model", model);

		if(amiguinho->is_grabbed())
		{
			amiguinho->use_texture("grabbed");
		}

		else if(amiguinho->is_moving())
		{
			amiguinho->use_texture("falling");
		}
		else {
			amiguinho->use_texture("default");
		}

		amiguinho->render_surface.render();
		shaders[0]->unbind();


	///
		ImGui::Begin("Amiguinho debug controls");

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	///

	amiguinho->display.swap_buffers();
	amiguinho->is_grounded_or_underground();
	amiguinho->mouse_interaction();
	
}


int main(int argc, char* argv[])
{
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);

	setup();

	while (!amiguinho->display.should_close())
	{
		render();
		glfwPollEvents();
	}

	glfwTerminate(); 

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}