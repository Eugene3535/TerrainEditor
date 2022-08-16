#include "ShaderProgram.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

ShaderProgram::ShaderProgram() noexcept:
	m_handle(0)
{
	m_handle = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	if (m_handle)
		glDeleteProgram(m_handle);
}

bool ShaderProgram::compileShader(const std::string& filename, GLenum type) noexcept
{
	std::string source;
	std::ifstream ifs_stream;

	ifs_stream.open(filename);

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

bool ShaderProgram::compileShaders(const std::string& filename_vert, const std::string& filename_frag) noexcept
{
	return compileShader(filename_vert, GL_VERTEX_SHADER) && compileShader(filename_frag, GL_FRAGMENT_SHADER);
}

bool ShaderProgram::compileShaders(const std::string& filename_vert, const std::string& filename_frag, const std::string& filename_geom) noexcept
{
	return compileShader(filename_vert, GL_VERTEX_SHADER) && compileShader(filename_frag, GL_FRAGMENT_SHADER) && compileShader(filename_geom, GL_GEOMETRY_SHADER);
}

bool ShaderProgram::addUniform(const std::string& name) noexcept
{
	GLint location = glGetUniformLocation(m_handle, name.c_str());

	if(location == -1)
		return false;

	return m_uniform_locations.try_emplace(name, location).second;
}
void ShaderProgram::setUniform(const std::string& name, float value) noexcept
{
	if(auto found = m_uniform_locations.find(name); found != m_uniform_locations.end())
		glUniform1f(found->second, value);
}
void ShaderProgram::setUniform(const std::string& name, const glm::vec3& vec) noexcept
{
	if(auto found = m_uniform_locations.find(name); found != m_uniform_locations.end())
		glUniform3f(found->second, vec.x, vec.y, vec.z);
}
void ShaderProgram::setUniform(const std::string& name, const float* matrix) noexcept
{
	if(auto found = m_uniform_locations.find(name); found != m_uniform_locations.end())
		glUniformMatrix4fv(found->second, 1, GL_FALSE, matrix);
}

void ShaderProgram::checkCompileErrors(GLuint handle, const std::string& type) noexcept
{
	GLint success;
	char infoLog[1024];

	if (type != "PROGRAM")
	{
		glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(handle, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
		}
	}
	else
	{
		glGetProgramiv(handle, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(handle, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
		}
	}
}

void ShaderProgram::use() noexcept
{
	glUseProgram(m_handle);
}