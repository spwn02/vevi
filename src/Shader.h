#pragma once

#include <iostream>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private:
	std::string m_filepath;
	unsigned int m_rendererID;
	std::unordered_map<std::string, int> m_uniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	// Set uniforms
	void setUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
private:
	ShaderProgramSource parseShader(const std::string& filepath);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

	unsigned int getUniformLocation(const std::string& name);
};