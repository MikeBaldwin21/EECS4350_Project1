#include "Shader.h"
#include <gl/glew.h>
#include <string>
#include <iostream>


Shader::Shader(const char* vertexShaderText, const char* fragmentShaderText)
{
	programID = -1;

	auto vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShaderID, 1, &vertexShaderText, NULL);
	glCompileShader(vertShaderID);

	{
		int success;
		char infoLog[512];
		glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(vertShaderID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	auto fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShaderID, 1, &fragmentShaderText, NULL);
	glCompileShader(fragShaderID);

	{
		int success;
		char infoLog[512];
		glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(fragShaderID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

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
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}

	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);
}

Shader::~Shader()
{
	glDeleteProgram(programID);
	programID = -1;
}

void Shader::Bind()
{
	glUseProgram(programID);
}

void Shader::Unbind()
{

}