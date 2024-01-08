#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Application.h"

OrthographicCamera::OrthographicCamera(float aspectRatio, float zoomLevel, float zNear, float zFar, bool isrotation)
	: m_AspectRatio(aspectRatio), m_ZoomLevel(zoomLevel), m_zNear(zNear), m_zFar(zFar), m_isRotation(isrotation)
	
{

}

void OrthographicCamera::OnUpdate(float ts)
{
	GLFWwindow* window = Application::Get().GetGLFWwindow();

	auto IsKeyDown = [&window](int key) ->int {
		return glfwGetKey(window, key) == GLFW_PRESS || glfwGetKey(window, key) == GLFW_REPEAT;
	};

	bool moved = false;
	float translationSpeed = m_ZoomLevel;
	float rotationSpeed = 180.0f;
	// Movement
	if (IsKeyDown(GLFW_KEY_W))
	{
		m_Position.x += -sin(glm::radians(m_Rotation)) * translationSpeed * ts;
		m_Position.y +=  cos(glm::radians(m_Rotation)) * translationSpeed * ts;
		moved = true;
	}
	else if (IsKeyDown(GLFW_KEY_S))
	{
		m_Position.x -= -sin(glm::radians(m_Rotation)) * translationSpeed * ts;
		m_Position.y -=  cos(glm::radians(m_Rotation)) * translationSpeed * ts;
		moved = true;
	}
	if (IsKeyDown(GLFW_KEY_A))
	{
		m_Position.x -= cos(glm::radians(m_Rotation)) * translationSpeed * ts;
		m_Position.y -= sin(glm::radians(m_Rotation)) * translationSpeed * ts;
		moved = true;
	}
	else if (IsKeyDown(GLFW_KEY_D))
	{
		m_Position.x += cos(glm::radians(m_Rotation)) * translationSpeed * ts;
		m_Position.y += sin(glm::radians(m_Rotation)) * translationSpeed * ts;
		moved = true;
	}

	if (m_Rotation)
	{
		if(IsKeyDown(GLFW_KEY_Q))
			m_Rotation += rotationSpeed * ts;
		if (IsKeyDown(GLFW_KEY_E))
			m_Rotation -= rotationSpeed * ts;

		if (m_Rotation > 180.0f)
			m_Rotation -= 360.0f;
		else if (m_Rotation <= -180.0f)
			m_Rotation += 360.0f;
	}

	RecalculateView();
	RecalculateProjection();
}

void OrthographicCamera::OnResize(uint32_t width, uint32_t height)
{
	m_AspectRatio = (float)width / (float)height;
	RecalculateProjection();
}



void OrthographicCamera::RecalculateProjection()
{
	m_Projection = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, m_zNear, m_zFar);
}

void OrthographicCamera::RecalculateView()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

	m_View = glm::inverse(transform);
}
