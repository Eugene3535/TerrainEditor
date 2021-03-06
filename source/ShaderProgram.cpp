#include "ShaderProgram.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

ShaderProgram::ShaderProgram():
	m_handle(0)
{
	m_handle = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	if (m_handle)
		glDeleteProgram(m_handle);
}

bool ShaderProgram::loadFromFile(const std::string& filepath, GLenum type)
{
	std::string source;
	std::ifstream ifs_stream;

	ifs_stream.open(filepath);

	if (ifs_stream.is_open())
	{
		std::stringstream s_stream;
		s_stream << ifs_stream.rdbuf();
		ifs_stream.close();

		source = s_stream.str();
	}
	else
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
		return false;
	}

	GLuint shader = glCreateShader(type);

	const GLchar* c_str = source.c_str();

	glShaderSource(shader, 1, &c_str, 0);
	glCompileShader(shader);
	checkCompileErrors(shader, "SHADER");

	glAttachShader(m_handle, shader);
	glLinkProgram(m_handle);
	checkCompileErrors(m_handle, "PROGRAM");

	glDeleteShader(shader);

	return true;
}

void ShaderProgram::use()
{
	glUseProgram(m_handle);
}

bool ShaderProgram::addUniform(const std::string& name)
{
	GLuint location = glGetUniformLocation(m_handle, name.c_str());

	return m_uniform_locations.try_emplace(name, location).second;
}

void ShaderProgram::setUniform(const std::string& name, float value)
{
	glUniform1f(m_uniform_locations[name], value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& vec)
{
	glUniform3f(m_uniform_locations[name], vec.x, vec.y, vec.z);
}

void ShaderProgram::setUniform(const std::string& name, const float* matrix)
{
	glUniformMatrix4fv(m_uniform_locations[name], 1, GL_FALSE, matrix);
}

void ShaderProgram::checkCompileErrors(GLuint handle, const std::string& type)
{
	int success;
	char infoLog[1024];

	if (type != "PROGRAM")
	{
		glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(handle, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(handle, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(handle, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}