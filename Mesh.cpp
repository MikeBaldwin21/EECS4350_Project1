#include "Mesh.h"
#include <gl/glew.h>
#include <vector>


Mesh::Mesh()
{
	vboID = -1;

	glGenBuffers(1, &vboID);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vboID);
	vboID = -1;
}

void Mesh::SetVertices(std::vector<float> verts)
{
	if(vboID < 0)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), &verts[0], GL_STATIC_DRAW);
}

void Mesh::Render()
{
}