#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.hpp"

class Camera
{	
public:
	Camera();
	Camera(const Camera& other) = delete;
	Camera(const Camera&& other) = delete;
	Camera& operator = (const Camera& other) = delete;
	Camera& operator = (const Camera&& other) = delete;
	~Camera();

	void init(ShaderProgram& shader);
	void apply(ShaderProgram& shader);

	void setMouseViewOrientation(float x, float y);
	void moveForward(float dt);
	void moveBackward(float dt);
	void strafeLeft(float dt);
	void strafeRight(float dt);
	void moveUp(float dt);
	void moveDown(float dt);
	void setMouseLook(bool toLook);
	const glm::mat4& getView() const;

private:
	glm::mat4 m_view;
	glm::vec3 m_position;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;
	glm::vec2 m_lastCoords;

	float m_pitch;
	float m_yaw;
	float m_velocity;
	float m_mouseSensitivity;

	bool m_is_firstMouseClick;
	bool m_is_LookAt;
};
