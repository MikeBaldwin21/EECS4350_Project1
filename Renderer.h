#pragma once

#include "Mesh.h"
#include "Shader.h"

class Renderer
{
private:
	Shader *shader;
	

public:
	Renderer(Shader *shader);
	void RenderMesh(Mesh *mesh);
};
