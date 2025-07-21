#include "Shader.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath)
	: m_filepath(filepath), m_rendererID(0)
{
    ShaderProgramSource source = parseShader(filepath);
    m_rendererID = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(m_rendererID);
}

ShaderProgramSource Shader::parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    if (!stream)
    {
        std::cout << "Failed to open a file " << filepath << '.' << std::endl;
        exit(-1);
    }

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            if (type == ShaderType::NONE) continue;
            ss[(int)type] << line << '\n';
        }
    }

    return ShaderProgramSource{ ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::bind() const
{
    glUseProgram(m_rendererID);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];

    int location = glGetUniformLocation(m_rendererID, name.c_str());

    if (location == -1)
        std::cout << "Warning: unform '" << name << "' doesn't exist!" << std::endl;

    m_uniformLocationCache[name] = location;
    return location;
}