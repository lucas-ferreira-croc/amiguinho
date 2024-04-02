#include "Texture.h"
#include <iostream>


Texture::Texture()
	: id(0), width(0), height(0), bit_depth(0), filepath("")
{
}


Texture::Texture(const char* filepath)
	: id(0), width(0), height(0), bit_depth(0), filepath(filepath)
{
}


Texture::~Texture()
{
	clear_texture();
}

bool Texture::load_texture()
{
	stbi_set_flip_vertically_on_load(true);

	unsigned char* tex_data = stbi_load(filepath, &width, &height, &bit_depth, 0);
	if (!tex_data)
	{
		std::cout << "Could not find: " << filepath << "\n";
		return false;
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(tex_data);

	return true;
}

bool Texture::load_textureA()
{
	stbi_set_flip_vertically_on_load(true);

	unsigned char* tex_data = stbi_load(filepath, &width, &height, &bit_depth, 0);
	if (!tex_data)
	{
		std::cout << "Could not find: " << filepath << "\n";
		return false;
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(tex_data);

	return true;
}

void Texture::use()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::clear_texture()
{
	glDeleteTextures(1, &id);
	id = 0;
	width = 0;
	height = 0;
	bit_depth = 0;
	filepath = (char*)"";
}