#include "GLHeaders.h"
#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shader::Shader() : programID(-1), varLocations() {}

Shader::Shader(std::string vertexShaderFilePath, std::string fragmentShaderFilePath) : programID(-1), varLocations()
{
	std::cout << "Building vertex shader from: " << vertexShaderFilePath << std::endl;
	auto vFile = std::ifstream(vertexShaderFilePath.c_str());
	std::string vertexText = std::string((std::istreambuf_iterator<char>(vFile)), std::istreambuf_iterator<char>());
	auto vStr = vertexText.c_str();

	auto vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShaderID, 1, &vStr, NULL);
	glCompileShader(vertShaderID);
	{
		int success;
		char infoLog[512];
		glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(vertShaderID, 512, NULL, infoLog);
			std::cout << "Vertex shader comilation failure:" << std::endl << infoLog << std::endl;
			glDeleteShader(vertShaderID);
			return;
		}
	}

	std::cout << "Vertex shader compilation successful" << std::endl;
	std::cout << "Building fragment shader from: " << fragmentShaderFilePath << std::endl;

	auto fFile = std::ifstream(fragmentShaderFilePath.c_str());
	std::string fragmentText = std::string((std::istreambuf_iterator<char>(fFile)), std::istreambuf_iterator<char>());
	auto fStr = fragmentText.c_str();

	auto fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShaderID, 1, &fStr, NULL);
	glCompileShader(fragShaderID);
	{
		int success;
		char infoLog[512];
		glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(fragShaderID, 512, NULL, infoLog);
			std::cout << "Fragment shader compilation failed:" << std::endl << infoLog << std::endl;
			glDeleteShader(fragShaderID);
			return;
		}
	}
	std::cout << "Fragment shader compilation successful" << std::endl;

	programID = glCreateProgram();
	glAttachShader(programID, vertShaderID);
	glAttachShader(programID, fragShaderID);
	glLinkProgram(programID);

	{
		int success;
		char infoLog[512];
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if(!success)
		{
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cout << "Shader program linking failed:" << std::endl << infoLog << std::endl;
			glDeleteProgram(programID);
			return;
		}
	}
	std::cout << "Shader program linking successful" << std::endl;

	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);

	GLint numberBlocks;
	char uniformName[1024];
	int nameLength;
	GLint size;
	GLenum type;
	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numberBlocks);
	std::cout << "List of shader uniform variables:" << std::endl;
	for(auto blockIndex = 0; blockIndex < numberBlocks; blockIndex++)
	{
		glGetActiveUniform(programID, blockIndex, 1024, &nameLength, &size, &type, uniformName);
		std::cout << uniformName << std::endl;
		varLocations[std::string(uniformName)] = blockIndex;
	}
}

void Shader::Free()
{
	glDeleteProgram(programID);
	programID = -1;
}

void Shader::Bind()
{
	glUseProgram(programID);
}