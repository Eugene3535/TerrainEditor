#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.hpp"


class Camera
{	
public:
	Camera()
	{
	}

	void init(ShaderProgram& shader)
	{		
		shader.addUniform("view");
	}

	void apply(ShaderProgram& shader)
	{
		m_view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader.setUniform("view", glm::value_ptr(m_view));
	}

	void mouseViewOrient(double xpos, double ypos)
	{
		if (mouselook)
		{
			if (firstMouseInput)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouseInput = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;
			lastX = xpos;
			lastY = ypos;

			xoffset *= mouseSens;
			yoffset *= mouseSens;

			yaw += xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 direction;
			direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			cameraFront = glm::normalize(direction);
		}
	}

	void moveForward(float dt)
	{
		cameraPos += speed * cameraFront * dt;

		//cameraPos.y = 3;
	};

	void moveBackward(float dt)
	{
		cameraPos -= speed * cameraFront * dt;
		//cameraPos.y = 3;
	};

	void strafeLeft(float dt)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * dt;
		//cameraPos.y = 3;
	};

	void strafeRight(float dt)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * dt;
		//cameraPos.y = 3;
	};

	void moveUp(float dt)
	{
		cameraPos += glm::vec3(0.f, speed * dt, 0.f);
	};

	void moveDown(float dt)
	{
		cameraPos -= glm::vec3(0.f, speed * dt, 0.f);
	};

	void setMouseLook(bool mouselook) 
	{
		this->mouselook = mouselook;

		if (mouselook == false) 
			firstMouseInput = true;
	}

	glm::mat4 getViewMat() { return m_view; }

private:
	bool mouselook = false;

	glm::mat4 m_view = glm::mat4(1);
	glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 4.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float pitch = 0, yaw = -70.f;
	float lastX = 1200 / 2, lastY = 800 / 2;


	float speed = 2;

	float mouseSens = 0.1f;

	bool firstMouseInput = true;
};
