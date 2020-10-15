#include "GLHeaders.h"
#include "Mesh.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <strstream>
#include <vector>

Mesh::Mesh() : vboID(-1), numOfTriangles(0), name() {}

Mesh::Mesh(std::string filePath) : vboID(-1), numOfTriangles(0), name(filePath)
{
	std::cout << "Loading mesh from file [" << filePath << "]" << std::endl;

	std::vector<float> vertices;
	std::vector<int> triangles;

	std::ifstream fileToRead;
	fileToRead.open(filePath.c_str());
	if(fileToRead.fail())
	{
		std::cout << "Failed to open \"" << filePath << std::endl;
		return;
	}

	// Put an dummy vertex in the zero spot in the vector. Then I don't need to adjust the indices in the OBJ file.
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(1);

	while(!fileToRead.fail() && !fileToRead.eof())
	{
		// read data in...
		std::string currentLine;
		std::string lineType;

		std::getline(fileToRead, currentLine);
		if(fileToRead.fail() || fileToRead.eof())
		{
			break;
		}

		auto lineRdr = std::istrstream(currentLine.c_str());
		lineRdr >> lineType;

		if(lineType == "v") // Vertex
		{
			float vertex[4];
			lineRdr >> vertex[0] >> vertex[1] >> vertex[2];
			vertex[3] = 1.0f;
			vertices.push_back(vertex[0]);
			vertices.push_back(vertex[1]);
			vertices.push_back(vertex[2]);
			vertices.push_back(vertex[3]);
		}
		else if(lineType == "vn") // Normal
		{
			continue;
		}
		else if(lineType == "f") // Face
		{
			char slash;
			int triangle[3];
			float triangleTexture[3];
			float triangleNormal[3];

			lineRdr >> triangle[0]; // First is always a vertex index
			if(lineRdr.peek() == '/')
			{
				lineRdr >> slash >> triangleTexture[0] >> slash >> triangleNormal[0] >> triangle[1] >> slash >> triangleTexture[1] >> slash >> triangleNormal[1] >> triangle[2] >> slash >> triangleTexture[2] >> slash >> triangleNormal[2];
				triangles.push_back(triangle[0]);
				triangles.push_back(triangle[1]);
				triangles.push_back(triangle[2]);
			}
			else
			{
				lineRdr >> triangle[1] >> triangle[2];
				triangles.push_back(triangle[0]);
				triangles.push_back(triangle[1]);
				triangles.push_back(triangle[2]);
			}
		}
		else
		{
			// Anything else like vt or s
			continue;
		}
	}

	// The file has been loaded, all the vertices and faces are loaded in RAM (within the std::vectors)

	// Build an array to hold the coordinates. Remember everything is a triangle.
	std::vector<float> triangleVertices = std::vector<float>();

	// Loop over all the triangle indices
	for(size_t i = 0; i < triangles.size(); i++)
	{
		// Grab the index that specifies a triangle vertex
		auto index = triangles[i];
		triangleVertices.push_back(vertices[index]);
		triangleVertices.push_back(vertices[index + 1]);
		triangleVertices.push_back(vertices[index + 2]);
		triangleVertices.push_back(vertices[index + 3]);
	}

	std::cout << "Mesh loaded. [" << vertices.size() << "] vetices, [" << triangles.size() << "] faces" << std::endl;

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* triangleVertices.size(), &triangleVertices[0], GL_STATIC_DRAW);

	numOfTriangles = triangleVertices.size() / 3;
}

void Mesh::Free()
{
	std::cout << "Freeing " << name << " mesh" << std::endl;
	numOfTriangles = 0;
	glDeleteBuffers(1, &vboID);
	vboID = -1;
}

unsigned int Mesh::GetID()
{
	return vboID;
}

void Mesh::Render()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glDrawArrays(GL_TRIANGLES, 0, numOfTriangles * 3);
}
