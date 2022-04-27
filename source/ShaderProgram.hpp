#pragma once

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <map>

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram operator = (const ShaderProgram& other) = delete;
	ShaderProgram(const ShaderProgram&& other) = delete;
	ShaderProgram operator = (const ShaderProgram&& other) = delete;
	~ShaderProgram();

	[[nodiscard]]
	bool loadFromFile(const std::string& path, GLenum type);

	void use();

	[[nodiscard]]
	bool addUniform(const std::string& name);

	void setUniform(const std::string& name, float value);
	void setUniform(const std::string& name, const glm::vec3& vec);
	void setUniform(const std::string& name, const float* matrix);

private:
	void checkCompileErrors(GLuint shader, const std::string& type);

	GLuint                        m_handle;
	std::map<std::string, GLuint> m_uniform_locations;
};