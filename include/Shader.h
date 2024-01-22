#pragma once

#include <iostream>
#include<fstream>
#include<string>  // getline‘⁄string¿Ô
#include<sstream>
#include<unordered_map>
#include"GL\glew.h"
#include"OpenGLDebug.h"
#include"vendor\glm\glm.hpp"



struct ShaderSource
{
	std::string VertexShader;

	std::string FragmentShader;
};

class Shader
{

private:
	unsigned int m_RendenID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformID;

public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3 );
	void SetUniformMat4v(const std::string& name, const glm::mat4& mat);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1i(const std::string& name, float value);


private:
	int GetUniformLocation(const std::string& name);
	unsigned int CreatShade(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int  type, const std::string& Source);
	ShaderSource ParseShader(const std::string& shaderPath);

};
