#include "Amiguinho.h"

#include <Windows.h>
#include <iostream>

#include "../meshes/Mesh.h"



Amiguinho::Amiguinho() : vertical_velocity(0.0f), gravity(0.5f), pos_x(0), pos_y(0), moving(false), grabbed(false)
{
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 1
	};

	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f
	};

	display = Display();
	display.initialize_window();

	render_surface = Mesh();
	render_surface.create_mesh(vertices, indices, 20, 6);
}

Amiguinho::~Amiguinho()
{
}


void Amiguinho::is_grounded_or_underground()
{
	if(pos_y <= display.get_mode()->height - MAX_GROUND_VALUE - 1)
	{
		fall();
		return;
	}

	vertical_velocity = 0.0f;
	pos_y = display.get_mode()->height - MAX_GROUND_VALUE;
	moving = false;
}

void Amiguinho::fall()
{
	moving = true;
	vertical_velocity += gravity;

	pos_y += vertical_velocity;

	display.set_window_pos(pos_x, pos_y);
}

void Amiguinho::mouse_interaction()
{
	if(glfwGetMouseButton(display.get_window(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		vertical_velocity = 0.0f;

		POINT cursorPos;
		GetCursorPos(&cursorPos);

		pos_x = cursorPos.x;
		pos_y = cursorPos.y;

		display.set_window_pos(pos_x, pos_y);

		grabbed = true;
		return;
	}

	grabbed = false;
}


bool Amiguinho::emplace_texture_map(std::string name, std::shared_ptr<Texture> texture)
{
	texture_map.insert(std::make_pair(name, texture));
	return true;
}

void Amiguinho::use_texture(std::string name)
{
	if(texture_map.find(name) != texture_map.end())
	{
		texture_map[name]->use();
	}
}