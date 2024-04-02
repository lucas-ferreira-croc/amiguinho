#pragma once

#include <GL/glew.h>
#include "../vendor/stb_image.h"


class Texture
{
public:
	Texture();
	Texture(const char* filepath);
	~Texture();

	bool load_texture();
	bool load_textureA();

	void use();
	void clear_texture();

	const char* get_filepath() { return filepath; };

private:

	unsigned int id;
	int width, height, bit_depth;

	const char* filepath;
};

