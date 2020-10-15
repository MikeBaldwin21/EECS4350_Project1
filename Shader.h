#pragma once

#include <map>
#include <string>

class Shader
{
public:
	unsigned int programID;
	std::map<std::string, int> varLocations;

public:
	Shader();
	Shader(std::string vertexShaderFilePath, std::string fragmentShaderFilePath);
	void Free();
	void Bind();
};