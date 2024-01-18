#include "Camera.h"

Camera::Camera(glm::vec3 position, float aspectRatio)
	:m_Position(position), m_AspectRatio(aspectRatio)
{
}

void Camera::OnUpdate(float ts)
{

}

void Camera::OnResize(uint32_t width, uint32_t height)
{
	if (width == m_ViewportWidth && height == m_ViewportHeight)
		return;
	m_AspectRatio = (float)width / (float)height;
	m_ViewportWidth = width;
	m_ViewportHeight = height;

	RecalculateRayDirections();
}

void Camera::RecalculateRayDirections()
{
	m_RayDirections.resize(m_ViewportWidth * m_ViewportHeight);
	for (uint32_t y = 0; y < m_ViewportHeight; y++)
	{
		for (uint32_t x = 0; x < m_ViewportWidth; x++)
		{
			glm::vec2 coord = { (float)x / (float)m_ViewportWidth, (float)y / (float)m_ViewportHeight };
			coord = coord * 2.0f - 1.0f; // -1 -> 1

			m_RayDirections[x + y * m_ViewportWidth] = glm::vec3(coord, -1.0f);
		}
	}
}
