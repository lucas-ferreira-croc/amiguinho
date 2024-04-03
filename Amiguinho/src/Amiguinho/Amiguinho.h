#pragma once

#include <unordered_map>
#include <string>

#include "../meshes/Mesh.h"
#include "../texture/Texture.h"
#include "../display/Display.h"

#include <vector>
#include <memory>

class Amiguinho
{
public:
	Amiguinho();
	~Amiguinho();

	Mesh render_surface;

	Display display;

	bool is_moving() { return moving; };

	bool is_grabbed() { return grabbed; }
private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> texture_map;

	unsigned int width;
	unsigned int height;

	int pos_x;
	int pos_y;

	float vertical_velocity;
	float gravity;

	bool moving;

	bool grabbed;
public:

	void is_grounded_or_underground();

	bool emplace_texture_map(std::string name, std::shared_ptr<Texture> texture);

	void use_texture(std::string name);

	void fall();

	void mouse_interaction();
};

