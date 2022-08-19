#include "Camera.hpp"

Camera::Camera():
    m_position(),
    m_cameraFront(),
    m_cameraUp(),
    m_lastCoords(),
	m_pitch(0),
	m_yaw(0),
	m_velocity(0),
	m_mouseSensitivity(0),
	m_is_firstMouseClick(true),
    m_is_LookAt(false)
{
}

Camera::~Camera()
{
}

void Camera::init(glm::mat4& view)
{
	m_view = &view;

	m_position = glm::vec3(5.0f, 5.0f, 8.0f);
	m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	m_pitch = 0;
	m_yaw = 0;
	m_lastCoords = { 1200 * 0.5f, 800 * 0.5f };

	m_velocity = 7;
	m_mouseSensitivity = 0.1f;
}

void Camera::apply(ShaderProgram& shader)
{
	if(m_view)
	{
		*m_view = glm::lookAt(m_position, m_position + m_cameraFront, m_cameraUp);
		shader.setUniform("view", glm::value_ptr(*m_view));
	}	
}

void Camera::setMouseViewOrientation(float x, float y)
{
	if (m_is_LookAt)
	{
		if (m_is_firstMouseClick)
		{
			m_lastCoords.x = x;
			m_lastCoords.y = y;
			m_is_firstMouseClick = false;
		}

		float offsetX = x - m_lastCoords.x;
		float offsetY = m_lastCoords.y - y;
		m_lastCoords.x = x;
		m_lastCoords.y = y;

		offsetX *= m_mouseSensitivity;
		offsetY *= m_mouseSensitivity;

		m_yaw += offsetX;
		m_pitch += offsetY;

		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		direction.y = sin(glm::radians(m_pitch));
		direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_cameraFront = glm::normalize(direction);
	}
}

void Camera::moveForward(float dt)
{
	m_position += m_velocity * m_cameraFront * dt;
}

void Camera::moveBackward(float dt)
{
	m_position -= m_velocity * m_cameraFront * dt;
}

void Camera::strafeLeft(float dt)
{
	m_position -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_velocity * dt;
}

void Camera::strafeRight(float dt)
{
	m_position += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_velocity * dt;
}

void Camera::moveUp(float dt)
{
	m_position += glm::vec3(0.f, m_velocity * dt, 0.f);
}

void Camera::moveDown(float dt)
{
	m_position -= glm::vec3(0.f, m_velocity * dt, 0.f);
};

void Camera::setMouseLook(bool look)
{
	this->m_is_LookAt = look;

	if (m_is_firstMouseClick == false)
		m_is_firstMouseClick = true;
}

