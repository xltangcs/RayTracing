#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Application.h"

#include <iostream>

Camera::Camera(float verticalFOV, float nearClip, float farClip)
	: m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip)
{
	m_ForwardDirection = glm::vec3(0, 0, -1);
	m_Position = glm::vec3(0, 0, 3);
}

void Camera::OnUpdate(float ts)
{
	GLFWwindow* window = Application::Get().GetGLFWwindow();

	auto IsKeyDown = [&window](int key) ->int {
		return glfwGetKey(window, key) == GLFW_PRESS || glfwGetKey(window, key) == GLFW_REPEAT;
	};

	double x, y;
	glfwGetCursorPos(window, &x, &y);

	glm::vec2 mousePos((float)x, (float)y);

	glm::vec2 delta = (mousePos - m_LastMousePosition) * 0.002f;
	m_LastMousePosition = mousePos;

	// mouse right PRESS status into editing mode
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return;
	}

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
	bool moved = false;

	constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
	glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

	float speed = 5.0f;

	// Movement
	if (IsKeyDown(GLFW_KEY_W))
	{
		m_Position += m_ForwardDirection * speed * ts;
		moved = true;
	}
	else if (IsKeyDown(GLFW_KEY_S))
	{
		m_Position -= m_ForwardDirection * speed * ts;
		moved = true;
	}
	if (IsKeyDown(GLFW_KEY_A))
	{
		m_Position -= rightDirection * speed * ts;
		moved = true;
	}
	else if (IsKeyDown( GLFW_KEY_D))
	{
		m_Position += rightDirection * speed * ts;
		moved = true;
	}

	if (IsKeyDown(GLFW_KEY_Q))
	{
		m_Position -= upDirection * speed * ts;
		moved = true;
	}
	else if (IsKeyDown(GLFW_KEY_E))
	{
		m_Position += upDirection * speed * ts;
		moved = true;
	}

	// Rotation
	if (delta.x != 0.0f || delta.y != 0.0f)
	{
		float pitchDelta = delta.y * GetRotationSpeed();
		float yawDelta = delta.x * GetRotationSpeed();

		glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection),
			glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
		m_ForwardDirection = glm::rotate(q, m_ForwardDirection);

		moved = true;
	}

	if (moved)
	{
		RecalculateView();
	}

}

void Camera::OnResize(uint32_t width, uint32_t height)
{
	if (width == m_ViewportWidth && height == m_ViewportHeight)
		return;

	m_ViewportWidth = width;
	m_ViewportHeight = height;

	RecalculateProjection();
}

float Camera::GetRotationSpeed()
{
	return 0.3f;
}

void Camera::RecalculateProjection()
{
	m_Projection = glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_ViewportWidth, (float)m_ViewportHeight, m_NearClip, m_FarClip);
	//m_Projection = glm::perspective(glm::radians(m_VerticalFOV), (float)m_ViewportWidth /(float)m_ViewportHeight, m_NearClip, m_FarClip);
	m_InverseProjection = glm::inverse(m_Projection);
}

void Camera::RecalculateView()
{
	m_View = glm::lookAt(m_Position, m_Position + m_ForwardDirection, glm::vec3(0, 1, 0));
	m_InverseView = glm::inverse(m_View);
}
