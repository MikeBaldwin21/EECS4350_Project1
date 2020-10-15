#pragma once

#include "GLHeaders.h"
#include <string>
#include <vector>

class Mesh
{
private:
	unsigned int vboID;
	unsigned int numOfTriangles;
	std::string name;

public:
	Mesh();
	Mesh(std::string filePath);
	void Free();
	unsigned int GetID();
	void Render();
};