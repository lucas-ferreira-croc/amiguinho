#pragma once

class Mesh
{
public:
	Mesh();
	~Mesh();


	void create_mesh(float* vertices, unsigned int* indices, unsigned int num_of_vertices, unsigned int num_of_indices);
	void render();

	void clear_mesh();
private:

	unsigned int vao;
	unsigned int vbo;
	unsigned int ibo;

	unsigned int index_count;

	float point_size;
};

