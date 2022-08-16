#pragma once

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <map>

class ShaderProgram
{
public:
	ShaderProgram() noexcept;
	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram(const ShaderProgram&& other) = delete;
	ShaderProgram& operator = (const ShaderProgram& other) = delete;
	ShaderProgram& operator = (const ShaderProgram&& other) = delete;
	~ShaderProgram();

	bool compileShader(const std::string& filename, GLenum type) noexcept;
	bool compileShaders(const std::string& filename_vert, const std::string& filename_frag) noexcept;
	bool compileShaders(const std::string& filename_vert, const std::string& filename_frag, const std::string& filename_geom) noexcept;
	
	bool addUniform(const std::string& name) noexcept;
	void setUniform(const std::string& name, float value) noexcept;
	void setUniform(const std::string& name, const glm::vec3& vec) noexcept;
	void setUniform(const std::string& name, const float* matrix) noexcept;
	void use() noexcept;

private:
	void checkCompileErrors(GLuint shader, const std::string& type) noexcept;

	GLuint                       m_handle;
	std::map<std::string, GLint> m_uniform_locations;
};