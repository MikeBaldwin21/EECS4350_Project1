#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"

Renderer::Renderer(Shader *shader)
{
	this->shader = shader;
}

void Renderer::RenderMesh(Mesh *mesh)
{
	shader->Bind();
	mesh->Render();
	shader->Unbind();
}