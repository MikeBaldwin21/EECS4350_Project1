#pragma once

#include <GL/glew.h>
#include <vector>

class Mesh
{
private:
	unsigned int vboID;

public:
	Mesh();
	~Mesh();
	void SetVertices(std::vector<float> verts);
	void Render();
};