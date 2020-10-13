#pragma once

#include <string>

class Shader
{
private:
	unsigned int programID;

public:
	Shader(const char *vertexShaderText, const char* fragmentShaderText);
	~Shader();
	void Bind();
	void Unbind();
};